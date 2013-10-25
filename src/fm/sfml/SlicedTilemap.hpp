#ifndef HEADER_ZN_SLICEDTILEMAP_HPP_INCLUDED
#define HEADER_ZN_SLICEDTILEMAP_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../util/Vector2.hpp"
#include "../util/Array2D.hpp"

namespace zn
{

// Optimized tilemap sliced into chunks. Empty tiles also don't generate geometry.
// Useful for big maps.
class SlicedTilemap : public sf::Drawable
{
public:

	SlicedTilemap(Vector2i tileSize, u32 chunkSize = 8);

	void build(const sf::Texture * atlas);

	zn::Array2D<s32> tiles;

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	struct Chunk
	{
		sf::VertexArray vertices;
	};

	const sf::Texture * r_atlas;
	u32 m_chunkSize;
	Vector2i m_tileSize;
	std::unordered_map<Vector2i,Chunk> m_chunks;

};

} // namespace zn

#endif // HEADER_ZN_SLICEDTILEMAP_HPP_INCLUDED
