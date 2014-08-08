#include <iostream>
#include <cassert>

#include <fm/asset/AssetBank.hpp>
#include <fm/json/json_utils.hpp>
#include <fm/scene/graphics/MapRenderer.hpp>
#include <fm/scene/core/Entity.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::MapRenderer)

//------------------------------------------------------------------------------
void MapRenderer::setAtlas(const TextureAtlas * atlas)
{
	r_atlas = atlas;
	onAtlasChanged(atlas);
}

//------------------------------------------------------------------------------
void MapRenderer::setTexture(const sf::Texture * texture)
{
	r_texture = texture;
}

//------------------------------------------------------------------------------
sf::FloatRect MapRenderer::localBounds() const
{
	return sf::FloatRect(0, 0,
		m_sceneTileSize.x * tiles.sizeX(),
		m_sceneTileSize.y * tiles.sizeY()
	);
}

//------------------------------------------------------------------------------
void MapRenderer::setTextureTileSize(sf::Vector2i sizePx)
{
#ifdef ZN_DEBUG
	if(r_atlas != nullptr && r_texture != nullptr)
	{
		if(sizePx.x > static_cast<s32>(r_texture->getSize().x) ||
			sizePx.y > static_cast<s32>(r_texture->getSize().y))
		{
			log.err() << "MapRenderer::setTextureTileSize: "
				"sizePx is too big : " << sizePx.x << "x" << sizePx.y << log.endl();
			return;
		}
		else if(sizePx.x <= 0 || sizePx.y <= 0)
		{
			log.err() << "MapRenderer::setTextureTileSize: "
				"invalid sizePx : " << sizePx.x << "x" << sizePx.y << log.endl();
			return;
		}
	}
#endif
	m_textureTileSize = sizePx;
}

//------------------------------------------------------------------------------
void MapRenderer::build(u32 sizeX, u32 sizeY, u32 fillTile)
{
	tiles.create(sizeX, sizeY, fillTile);
	updateMesh();
}

//------------------------------------------------------------------------------
void MapRenderer::build(const std::string & mapName,
						const std::string & textureName,
						const std::string & atlasName,
						const std::string layerName,
						const std::string tilesetName)
{
	AssetBank & assets = *AssetBank::current();
	const TiledMap * map = assets.maps.get(mapName);
	const sf::Texture * texture = assets.textures.get(textureName);
	const TextureAtlas * atlas = assets.atlases.get(atlasName);
	build(map, texture, atlas, layerName, tilesetName);
}

//------------------------------------------------------------------------------
void MapRenderer::build(const TiledMap * map,
						const sf::Texture * texture,
						const TextureAtlas * atlas,
						const std::string layerName,
						const std::string tilesetName)
{
	// Reference map
	r_tiledMap = map;

	// If a layer is specified
	if(!layerName.empty())
	{
		// Set new layer name
		m_tiledLayerName = layerName;
	}

	// If a tileset is specified
	if(!tilesetName.empty())
	{
		// Set new tileset name
		m_tiledTilesetName = tilesetName;
	}

	// If a new texture is specified
	if(texture != nullptr)
	{
		// Set texture
		r_texture = texture;
	}

	// If a new atlas is specified
	if(atlas != nullptr)
	{
		// Set atlas
		r_atlas = atlas;
	}

	// Check existence of a layer
	if(r_tiledMap->layers.empty())
	{
		log.err() << "MapRenderer::build: no layer in TiledMap !" << log.endl();
		return;
	}

	// Check existence of a tileset
	if(r_tiledMap->tileSets.empty())
	{
		log.err() << "MapRenderer::build: no tileset in TiledMap !" << log.endl();
		return;
	}

	// Get layer
	const TiledMap::Layer * layer = nullptr;
	if(m_tiledLayerName.empty())
	{
		layer = &(r_tiledMap->layers[0]); // Get default layer if none specified
	}
	else
	{
		layer = r_tiledMap->layerFromName(m_tiledLayerName);
#ifdef ZN_DEBUG
		if(layer == nullptr)
		{
			log.err() << "MapRenderer::build: TiledMap layer not found : \""
				<< m_tiledLayerName << '"' << log.endl();
		}
#endif
	}

	assert(layer != nullptr);

	// Get tileset
	const TiledMap::TileSet* tileset = nullptr;
	if(m_tiledTilesetName.empty())
	{
		tileset = &(r_tiledMap->tileSets[0]); // Get default tileset if none specified
	}
	else
	{
		tileset = r_tiledMap->tileSetFromName(m_tiledTilesetName);
	}

	// Copy tiles data
	tiles.create(r_tiledMap->size.x, r_tiledMap->size.y, 0);
	for(s32 y = 0; y < layer->size.y; ++y)
	{
		for(s32 x = 0; x < layer->size.x; ++x)
		{
			s32 tile = (s32)layer->at(x, y) - (s32)tileset->firstgid;

			s32 absX = layer->position.x+x;
			s32 absY = layer->position.y+y;

			if(tiles.contains(absX, absY))
			{
				tiles.setNoEx(absX, absY, tile);
			}
		}
	}

	setTextureTileSize(r_tiledMap->tileSize);

	updateMesh();
}

//------------------------------------------------------------------------------
void MapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	if(r_texture == nullptr)
		log.err() << "MapRenderer::draw: no atlas defined !" << log.endl();
#endif

	if(entity().transform())
	{
		states.transform.combine(entity().transform()->matrix());
	}

	// apply the texture
	states.texture = r_texture;

	// draw the vertex array
	target.draw(m_vertices, states);
}

//------------------------------------------------------------------------------
void MapRenderer::updateMesh()
{
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(tiles.sizeX() * tiles.sizeY() * 4);

#ifdef ZN_DEBUG
	log.debug() << "MapRenderer: update monolithic mesh ("
		<< tiles.sizeX() << "x" << tiles.sizeY() << ")" << log.endl();
#endif

	// populate the vertex array, with one quad per tile
	for(u32 j = 0; j < tiles.sizeY(); ++j)
	{
		for(u32 i = 0; i < tiles.sizeX(); ++i)
		{
			updateTile(i, j);
		}
	}
}

//------------------------------------------------------------------------------
void MapRenderer::updateTile(u32 i, u32 j)
{
	// Note: slight padding of tile texture coordinates eliminates artifacts
	// when the map is not rendered on an integer scale (or rotated)
	const f32 pad = 0.45f;

	const sf::Texture & texture = *r_texture;

	// get the current tile number
	s32 tileNumber = tiles.getNoEx(i, j);

	// get a pointer to the current tile's quad
	sf::Vertex * quad = &m_vertices[(i + j * tiles.sizeX()) * 4];

	if(tileNumber >= 0)
	{
		// define its 4 corners
		quad[0].position = sf::Vector2f(i * m_sceneTileSize.x, j * m_sceneTileSize.y);
		quad[1].position = sf::Vector2f((i + 1) * m_sceneTileSize.x, j * m_sceneTileSize.y);
		quad[2].position = sf::Vector2f((i + 1) * m_sceneTileSize.x, (j + 1) * m_sceneTileSize.y);
		quad[3].position = sf::Vector2f(i * m_sceneTileSize.x, (j + 1) * m_sceneTileSize.y);

		// find its position in the tileset texture
		s32 tu = tileNumber % (texture.getSize().x / m_textureTileSize.x);
		s32 tv = tileNumber / (texture.getSize().y / m_textureTileSize.y);

		// define its 4 texture coordinates
		quad[0].texCoords = sf::Vector2f(tu * m_textureTileSize.x + pad, tv * m_textureTileSize.y + pad);
		quad[1].texCoords = sf::Vector2f((tu + 1) * m_textureTileSize.x - pad, tv * m_textureTileSize.y + pad);
		quad[2].texCoords = sf::Vector2f((tu + 1) * m_textureTileSize.x - pad, (tv + 1) * m_textureTileSize.y - pad);
		quad[3].texCoords = sf::Vector2f(tu * m_textureTileSize.x + pad, (tv + 1) * m_textureTileSize.y - pad);
	}
	else
	{
		// Null coordinates (will not fill any pixel, stills better than lots of alpha quads)
		quad[0].position = sf::Vector2f();
		quad[1].position = sf::Vector2f();
		quad[2].position = sf::Vector2f();
		quad[3].position = sf::Vector2f();

		// Reset texcoords
		quad[0].texCoords = sf::Vector2f();
		quad[1].texCoords = sf::Vector2f();
		quad[2].texCoords = sf::Vector2f();
		quad[3].texCoords = sf::Vector2f();
	}
}

//------------------------------------------------------------------------------
void MapRenderer::setTile(u32 x, u32 y, s32 tileNumber)
{
	tiles.set(x, y, tileNumber);
	updateTile(x, y);
}

//------------------------------------------------------------------------------
void MapRenderer::serializeData(JsonBox::Value & o)
{
	Renderer::serializeData(o);

	// Tiled data

	std::string tiledMapName;
	if(r_tiledMap != nullptr)
	{
		tiledMapName = AssetBank::current()->maps.findName(r_tiledMap);
	}
	o["tiledMap"]      = tiledMapName;
	o["tiledLayer"]    = m_tiledLayerName;
	o["tiledTileset"]  = m_tiledTilesetName;

	// Atlas

	std::string atlasName;
	if(r_atlas != nullptr)
	{
		atlasName = AssetBank::current()->atlases.findName(r_atlas);
	}
	o["atlas"] = atlasName;

	// Texture

	std::string textureName;
	if(r_texture != nullptr)
	{
		textureName = AssetBank::current()->textures.findName(r_texture);
	}
	o["texture"] = textureName;

	// Tiles

	zn::serialize(o["textureTileSize"], m_textureTileSize);
	zn::serialize(o["sceneTileSize"], m_sceneTileSize);

	zn::serialize(o["size"], sf::Vector2i(tiles.sizeX(), tiles.sizeY()));

	// TODO tweak JsonBox to output large arrays of numbers in line or without indentation in readable mode,
	// because indentation is heavy as hell and the size make the file uneasy to navigate
	JsonBox::Array tilesData(tiles.area());
	for(u32 i = 0; i < tilesData.size(); ++i)
	{
		tilesData[i] = tiles[i];
	}
	o["tilesData"] = tilesData;
}

//------------------------------------------------------------------------------
void MapRenderer::unserializeData(JsonBox::Value & o)
{
	Renderer::unserializeData(o);

	// Tiled data

	std::string tiledMapName = o["tiledMap"].getString();
	m_tiledLayerName   = o["tiledLayer"].getString();
	m_tiledTilesetName = o["tiledTileset"].getString();
	r_tiledMap = AssetBank::current()->maps.get(tiledMapName);

	// Atlas

	std::string atlasName = o["atlas"].getString();
#ifdef ZN_DEBUG
	if(atlasName.empty())
		log.warn() << "MapRenderer::unserializeData: atlas is empty" << log.endl();
#endif
	r_atlas = AssetBank::current()->atlases.get(atlasName);

	// Atlas

	std::string textureName = o["texture"].getString();
#ifdef ZN_DEBUG
	if(textureName.empty())
		log.warn() << "MapRenderer::unserializeData: texture is empty" << log.endl();
#endif
	r_texture = AssetBank::current()->textures.get(textureName);

	// Tiles

	zn::unserialize(o["textureTileSize"], m_textureTileSize);
	zn::unserialize(o["sceneTileSize"], m_sceneTileSize);

	sf::Vector2i size;
	zn::unserialize(o["size"], size);
	tiles.create(size.x, size.y);

	JsonBox::Value & data = o["tilesData"];
	const u32 n = data.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		tiles[i] = data[i].getInt();
	}
}

} // namespace zn

