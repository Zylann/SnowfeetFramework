#include <iostream>

#include "MapRenderer.hpp"
#include "../../asset/AssetBank.hpp"

namespace zn
{

void MapRenderer::setAtlas(const TextureAtlas * atlas)
{
	r_atlas = atlas;
	m_needUpdate = true;
}

//------------------------------------------------------------------------------
void MapRenderer::setMap(const TiledMap* map, const TextureAtlas * atlas, const std::string bgLayerName)
{
	r_tiledMap = map;
	m_bgLayerName = bgLayerName;
	if(atlas != nullptr)
		r_atlas = atlas;
	m_needUpdate = true;
}

//------------------------------------------------------------------------------
void MapRenderer::onUpdate()
{
	if(m_needUpdate)
		updateMesh();
}

//------------------------------------------------------------------------------
void MapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	if(r_atlas == nullptr)
		std::cout << "E: MapRenderer::draw: no atlas defined !" << std::endl;
#endif
	// apply the tileset texture
	states.texture = &r_atlas->texture();

	// draw the vertex array
	target.draw(m_vertices, states);
}

//------------------------------------------------------------------------------
void MapRenderer::updateMesh()
{
	if(r_atlas == nullptr || r_tiledMap == nullptr)
	{
#ifdef ZN_DEBUG
		std::cout << "W: MapRenderer::updateMesh: no atlas or no map defined." << std::endl;
#endif
		return;
	}

	const TiledMap::Layer * layer = r_tiledMap->layerFromName(m_bgLayerName);
	if(layer == nullptr)
	{
		std::cout << "E: MapRenderer::updateMesh: "
			"the given TiledMap doesn't have any layer named \"" << m_bgLayerName << "\"" << std::endl;
		m_needUpdate = false;
		return;
	}

	const sf::Texture & texture = r_atlas->texture();
	sf::Vector2i tileSize = r_tiledMap->tileSize;
	u32 width = layer->size.x;
	u32 height = layer->size.y;

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

#ifdef ZN_DEBUG
	std::cout << "D: MapRenderer: update monolithic mesh (" << width << "x" << height << ")" << std::endl;
#endif

	// populate the vertex array, with one quad per tile
	for(u32 j = 0; j < height; ++j)
	{
		for(u32 i = 0; i < width; ++i)
		{
			// get the current tile number
			s32 tileNumber = layer->at(i,j) - r_tiledMap->tileSets[0].firstgid;

			// find its position in the tileset texture
			s32 tu = tileNumber % (texture.getSize().x / tileSize.x);
			s32 tv = tileNumber / (texture.getSize().y / tileSize.y);

			// get a pointer to the current tile's quad
			sf::Vertex * quad = &m_vertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}

	m_needUpdate = false;
}

//------------------------------------------------------------------------------
void MapRenderer::serializeData(JsonBox::Value & o)
{
	ARenderer::serializeData(o);

	// TODO support TiledMap that are not from assets?
	// This is possible as long as the user maintains the data available,
	// but it will not be saved here. The user will have to handle saving
	// by writing its own component and serialization.

	std::string tiledMapName;
	if(r_tiledMap != nullptr)
	{
		tiledMapName = AssetBank::current()->maps.findName(r_tiledMap);
	}

	std::string atlasName;
	if(r_atlas != nullptr)
	{
		atlasName = AssetBank::current()->atlases.findName(r_atlas);
	}

	o["tiledMap"]      = tiledMapName;
	o["atlas"]         = atlasName;
	o["bgLayerName"]   = m_bgLayerName;
}

//------------------------------------------------------------------------------
void MapRenderer::unserializeData(JsonBox::Value & o)
{
	ARenderer::unserializeData(o);

	std::string tiledMapName  = o["tiledMap"].getString();
	std::string atlasName     = o["atlas"].getString();
	std::string bgLayerName   = o["bgLayerName"].getString();

#ifdef ZN_DEBUG
	if(tiledMapName.empty())
		std::cout << "W: MapRenderer::unserializeData: tiledMapName is empty" << std::endl;
	if(atlasName.empty())
		std::cout << "W: MapRenderer::unserializeData: atlasName is empty" << std::endl;
	if(bgLayerName.empty())
		std::cout << "W: MapRenderer::unserializeData: bgLayerName is empty" << std::endl;
#endif

	r_tiledMap = AssetBank::current()->maps.get(tiledMapName);
	r_atlas = AssetBank::current()->atlases.get(atlasName);
	m_bgLayerName = bgLayerName;
	m_needUpdate = true;
}

//------------------------------------------------------------------------------
void MapRenderer::postUnserialize()
{

}

} // namespace zn

