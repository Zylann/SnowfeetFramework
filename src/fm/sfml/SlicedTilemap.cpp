#include "SlicedTilemap.hpp"

namespace zn
{

SlicedTilemap::SlicedTilemap(Vector2i tileSize, u32 chunkSize) :
	m_chunkSize(chunkSize),
	m_tileSize(tileSize)
{
}

void SlicedTilemap::build(const sf::Texture * atlas)
{
	std::cout << "D: SlicedTilemap: building tiles..." << std::endl;

	r_atlas = atlas;

	m_chunks.clear();

	const s32 atlasNtilesX = (r_atlas->getSize().x / m_tileSize.x);
	//const s32 atlasNtilesY = (r_atlas->getSize().y / m_tileSize.y); // Not needed

	const sf::Color defaultColor(255,255,255,255);

	const u32 nchunksX = tiles.sizeX()/m_chunkSize+1;
	const u32 nchunksY = tiles.sizeY()/m_chunkSize+1;

	u32 ntiles = 0;

	for(u32 cy = 0; cy < nchunksY; ++cy)
	{
		for(u32 cx = 0; cx < nchunksX; ++cx)
		{
			Chunk chunk;
			chunk.vertices.setPrimitiveType(sf::Quads);

			for(u32 ry = 0; ry < m_chunkSize; ++ry)
			{
				for(u32 rx = 0; rx < m_chunkSize; ++rx)
				{
					u32 x = cx * m_chunkSize + rx;
					u32 y = cy * m_chunkSize + ry;

					if(x < tiles.sizeX() && y < tiles.sizeY())
					{
						s32 tile = tiles.getNoEx(x, y);

						if(tile >= 0)
						{
							chunk.vertices.resize(chunk.vertices.getVertexCount() + 4);
							sf::Vertex * quad = &chunk.vertices[chunk.vertices.getVertexCount() - 4];

							quad[0].position = sf::Vector2f(x, y);
							quad[1].position = sf::Vector2f(x+1, y);
							quad[2].position = sf::Vector2f(x+1, y+1);
							quad[3].position = sf::Vector2f(x, y+1);

							quad[0].color = defaultColor;
							quad[1].color = defaultColor;
							quad[2].color = defaultColor;
							quad[3].color = defaultColor;

							s32 tileX = tile % atlasNtilesX;
							s32 tileY = tile / atlasNtilesX;

							quad[0].texCoords = sf::Vector2f(
									static_cast<f32>(tileX) * m_tileSize.x,
									static_cast<f32>(tileY) * m_tileSize.y
							);

							quad[1].texCoords = sf::Vector2f(
									static_cast<f32>(tileX+1) * m_tileSize.x,
									static_cast<f32>(tileY) * m_tileSize.y
							);

							quad[2].texCoords = sf::Vector2f(
									static_cast<f32>(tileX+1) * m_tileSize.x,
									static_cast<f32>(tileY+1) * m_tileSize.y
							);

							quad[3].texCoords = sf::Vector2f(
									static_cast<f32>(tileX) * m_tileSize.x,
									static_cast<f32>(tileY+1) * m_tileSize.y
							);

							++ntiles;
						}
					}
				}
			}

			if(chunk.vertices.getVertexCount() > 0)
			{
				m_chunks[Vector2i(cx,cy)] = chunk;
			}
		}
	}

	std::cout << "D: SlicedTilemap: done (" << m_chunks.size() << " chunks, " << ntiles << " tiles)" << std::endl;
}

void SlicedTilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the tileset texture
	states.texture = r_atlas;

	for(auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
	{
		target.draw(it->second.vertices, states);
	}
}

} // namespace zn


