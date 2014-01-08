#ifndef HEADER_ZN_MAPRENDERER_HPP_INCLUDED
#define HEADER_ZN_MAPRENDERER_HPP_INCLUDED

#include "Renderer.hpp"
#include "../../asset/TiledMap.hpp"
#include "../../util/Array2D.hpp"

namespace zn
{

// 2D orthographic map renderer.
// It renders only one layer of tiles.
class MapRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::MapRenderer, CG_RENDERER)

	MapRenderer() : ARenderer(),
		r_atlas(nullptr),
		m_tileSize(32,32),
		r_tiledMap(nullptr)
	{}

	void setAtlas(const TextureAtlas * atlas) override;

	// Setups the tilemap from a TiledMap
	void setMap(const TiledMap * map,
				const TextureAtlas * atlas = nullptr,
				const std::string layerName="",
				const std::string tilesetName="");

	// Serialize
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

protected:

	void onUpdate() override;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	// Rebuilds the whole vertex array
	void updateMesh();

	// Updates texture coordinates of one quad.
	// Coordinates are supposed to be valid.
	void updateTile(u32 i, u32 j);

private:

	const TextureAtlas *   r_atlas;
	zn::Array2D<s32>       m_tilesData; // negative value means no tile
	sf::VertexArray        m_vertices;
	sf::Vector2i           m_tileSize;

	const TiledMap *       r_tiledMap;
	std::string            m_tiledLayerName;
	std::string            m_tiledTilesetName;

};

} // namespace zn

#endif // HEADER_ZN_MAPRENDERER_HPP_INCLUDED


