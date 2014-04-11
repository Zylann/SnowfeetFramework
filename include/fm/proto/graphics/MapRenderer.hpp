#ifndef HEADER_ZN_MAPRENDERER_HPP_INCLUDED
#define HEADER_ZN_MAPRENDERER_HPP_INCLUDED

#include <fm/proto/graphics/Renderer.hpp>
#include <fm/asset/TiledMap.hpp>
#include <fm/util/Array2D.hpp>

namespace zn
{

// 2D orthographic map renderer.
// It renders only one layer of tiles.
class ZN_API MapRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::MapRenderer, CG_RENDERER, 0)

	MapRenderer() : ARenderer(),
		r_atlas(nullptr),
		r_texture(nullptr),
		m_tileSize(32,32),
		r_tiledMap(nullptr)
	{}

	void setAtlas(const TextureAtlas * atlas) override;
	const TextureAtlas * atlas() const override { return r_atlas; }

	void setTexture(const sf::Texture * texture) override;
	const sf::Texture * texture() const override { return r_texture; }

	sf::FloatRect localBounds() const override;

	void build(u32 sizeX, u32 sizeY, u32 fillTile=0);

	// Setups the tilemap from a TiledMap
	void build(const TiledMap * map,
				const sf::Texture * texture = nullptr,
				const TextureAtlas * atlas = nullptr,
				const std::string layerName="",
				const std::string tilesetName="");

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

	// Sets the size of a tile in pixels.
	// You must call updateMesh() after doing this.
	void setTileSize(sf::Vector2i sizePx);

	// Get the size of a tile in pixels
	const sf::Vector2i tileSize() const { return m_tileSize; }

	// Sets the tile at (x,y) to a new number.
	// Calls updateTile(x,y).
	// Coordinates must be valid.
	void setTile(u32 x, u32 y, s32 tileNumber);

	// Rebuilds the whole vertex array and updates all tiles.
	// Gets called automatically by setMap().
	void updateMesh();

	// Updates texture coordinates of one quad.
	// Coordinates are supposed to be valid.
	void updateTile(u32 i, u32 j);

	// 2D array of tile indexes, exposed for convenience.
	// You must call updateTile(x,y) for each tile you modify from here,
	// Or updateMesh() if you modify the size too. If you don't, undefined behaviour may happen.
	// Negative values are interpreted as "no tile".
	zn::Array2D<s32> tiles;

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	const TextureAtlas *   r_atlas;
	const sf::Texture *    r_texture;
	sf::VertexArray        m_vertices;
	sf::Vector2i           m_tileSize;

	const TiledMap *       r_tiledMap;
	std::string            m_tiledLayerName;
	std::string            m_tiledTilesetName;

};

} // namespace zn

#endif // HEADER_ZN_MAPRENDERER_HPP_INCLUDED


