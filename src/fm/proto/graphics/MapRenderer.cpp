#include <iostream>
#include <cassert>

#include "MapRenderer.hpp"
#include "../../asset/AssetBank.hpp"
#include "../Entity.hpp"
#include "../../json/json_utils.hpp"

namespace zn
{

void MapRenderer::setAtlas(const TextureAtlas * atlas)
{
	r_atlas = atlas;
}

//------------------------------------------------------------------------------
void MapRenderer::setMap(const TiledMap * map,
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

	// If a new atlas is specified
	if(atlas != nullptr)
	{
		// Set atlas
		r_atlas = atlas;
	}

	// Check existence of a layer
	if(r_tiledMap->layers.empty())
	{
		std::cout << "E: MapRenderer::setMap: no layer in TiledMap !" << std::endl;
		return;
	}

	// Check existence of a tileset
	if(r_tiledMap->tileSets.empty())
	{
		std::cout << "E: MapRenderer::setMap: no tileset in TiledMap !" << std::endl;
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
			std::cout << "E: MapRenderer::setMap: TiledMap layer not found : \""
				<< m_tiledLayerName << '"' << std::endl;
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
	m_tilesData.create(r_tiledMap->size.x, r_tiledMap->size.y, 0);
	for(s32 y = 0; y < layer->size.y; ++y)
	{
		for(s32 x = 0; x < layer->size.x; ++x)
		{
			s32 tile = (s32)layer->at(x, y) - (s32)tileset->firstgid;

			s32 absX = layer->position.x+x;
			s32 absY = layer->position.y+y;

			if(m_tilesData.contains(absX, absY))
			{
				m_tilesData.setNoEx(absX, absY, tile);
			}
		}
	}

	updateMesh();
}

//------------------------------------------------------------------------------
void MapRenderer::onUpdate()
{
}

//------------------------------------------------------------------------------
void MapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	if(r_atlas == nullptr)
		std::cout << "E: MapRenderer::draw: no atlas defined !" << std::endl;
#endif

	states.transform.combine(entity().matrix());

	// apply the tileset texture
	states.texture = &r_atlas->texture();

	// draw the vertex array
	target.draw(m_vertices, states);
}

//------------------------------------------------------------------------------
void MapRenderer::updateMesh()
{
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(m_tilesData.sizeX() * m_tilesData.sizeY() * 4);

#ifdef ZN_DEBUG
	std::cout << "D: MapRenderer: update monolithic mesh ("
		<< m_tilesData.sizeX() << "x" << m_tilesData.sizeY() << ")"
		<< std::endl;
#endif

	// populate the vertex array, with one quad per tile
	for(u32 j = 0; j < m_tilesData.sizeY(); ++j)
	{
		for(u32 i = 0; i < m_tilesData.sizeX(); ++i)
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
	const f32 pad = 0.01f;

	const sf::Texture & texture = r_atlas->texture();

	// get the current tile number
	s32 tileNumber = m_tilesData.getNoEx(i, j);

	// get a pointer to the current tile's quad
	sf::Vertex * quad = &m_vertices[(i + j * m_tilesData.sizeX()) * 4];

	if(tileNumber >= 0)
	{
		// find its position in the tileset texture
		s32 tu = tileNumber % (texture.getSize().x / m_tileSize.x);
		s32 tv = tileNumber / (texture.getSize().y / m_tileSize.y);

		// define its 4 corners
		quad[0].position = sf::Vector2f(i * m_tileSize.x, j * m_tileSize.y);
		quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
		quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
		quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);

		// define its 4 texture coordinates
		quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x + pad, tv * m_tileSize.y + pad);
		quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x - pad, tv * m_tileSize.y + pad);
		quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x - pad, (tv + 1) * m_tileSize.y - pad);
		quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x + pad, (tv + 1) * m_tileSize.y - pad);
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
void MapRenderer::serializeData(JsonBox::Value & o)
{
	ARenderer::serializeData(o);

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
	o["atlas"]         = atlasName;

	// Tiles

	zn::serialize(o["tileSize"], m_tileSize);

	zn::serialize(o["size"], sf::Vector2i(m_tilesData.sizeX(), m_tilesData.sizeY()));

	// TODO tweak JsonBox to output large arrays of numbers in line or without indentation in readable mode,
	// because indentation is heavy as hell and the size make the file uneasy to navigate
	JsonBox::Array tilesData(m_tilesData.area());
	for(u32 i = 0; i < tilesData.size(); ++i)
	{
		tilesData[i] = m_tilesData[i];
	}
	o["tilesData"] = tilesData;
}

//------------------------------------------------------------------------------
void MapRenderer::unserializeData(JsonBox::Value & o)
{
	ARenderer::unserializeData(o);

	// Tiled data

	std::string tiledMapName = o["tiledMap"].getString();
	m_tiledLayerName   = o["tiledLayer"].getString();
	m_tiledTilesetName = o["tiledTileset"].getString();
	r_tiledMap = AssetBank::current()->maps.get(tiledMapName);

	// Atlas

	std::string atlasName = o["atlas"].getString();
#ifdef ZN_DEBUG
	if(atlasName.empty())
		std::cout << "W: MapRenderer::unserializeData: atlasName is empty" << std::endl;
#endif
	TextureAtlas * atlas = AssetBank::current()->atlases.get(atlasName);
	r_atlas = atlas;

	// Tiles

	zn::unserialize(o["tileSize"], m_tileSize);

	sf::Vector2i size;
	zn::unserialize(o["size"], size);
	m_tilesData.create(size.x, size.y);

	JsonBox::Value & data = o["tilesData"];
	const u32 n = data.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		m_tilesData[i] = data[i].getInt();
	}
}

//------------------------------------------------------------------------------
void MapRenderer::postUnserialize()
{

}

} // namespace zn

