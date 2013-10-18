/*
Tilemap.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include "Tilemap.hpp"
#include <iostream>

namespace zn
{

void Tilemap::create(u32 width, u32 height)
{
	m_size.set(width, height);

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(m_size.x * m_size.y * 4);

	sf::Color defaultColor = sf::Color(255, 255, 255, 255);

	u32 i = 0;

	for(s32 y = 0; y < m_size.y; ++y)
	{
		for(s32 x = 0; x < m_size.x; ++x)
		{
			sf::Vertex * quad = &m_vertices[i];

			quad[0].position = sf::Vector2f(x, y);
			quad[1].position = sf::Vector2f(x+1, y);
			quad[2].position = sf::Vector2f(x+1, y+1);
			quad[3].position = sf::Vector2f(x, y+1);

			quad[0].color = defaultColor;
			quad[1].color = defaultColor;
			quad[2].color = defaultColor;
			quad[3].color = defaultColor;

			i += 4;
		}
	}
}

void Tilemap::setAtlas(const sf::Texture* atlas, u32 tileSize)
{
	setAtlas(atlas, Vector2i(tileSize, tileSize));
}

void Tilemap::setAtlas(const sf::Texture * atlas, Vector2i tileSize)
{
	r_atlas = atlas;
	m_tileSize = tileSize;
//	m_normalizedTileSize.x = static_cast<f32>(m_tileSize.x) / static_cast<f32>(r_atlas->getSize().x);
//	m_normalizedTileSize.y = static_cast<f32>(m_tileSize.y) / static_cast<f32>(r_atlas->getSize().y);
}

bool Tilemap::contains(s32 x, s32 y)
{
	return x >= 0 && y >= 0 && x < m_size.x && y < m_size.y;
}

void Tilemap::setTile(s32 x, s32 y, u32 tile)
{
	sf::Vector2u atlasSize = r_atlas->getSize();
	setTile(x, y,
		tile % (atlasSize.x / m_tileSize.x),
		tile / (atlasSize.x / m_tileSize.x)
	);
}

void Tilemap::setTile(s32 x, s32 y, u32 tileX, u32 tileY)
{
	sf::Vertex * quad = &m_vertices[(y * m_size.x + x) * 4];

	// Note: SFML Graphics works with pixel texture coordinates.
	sf::Vector2f s = sf::Vector2f(m_tileSize.x, m_tileSize.y);

	quad[0].texCoords = sf::Vector2f(
			static_cast<f32>(tileX) * s.x,
			static_cast<f32>(tileY) * s.y
	);

	quad[1].texCoords = sf::Vector2f(
			static_cast<f32>(tileX+1) * s.x,
			static_cast<f32>(tileY) * s.y
	);

	quad[2].texCoords = sf::Vector2f(
			static_cast<f32>(tileX+1) * s.x,
			static_cast<f32>(tileY+1) * s.y
	);

	quad[3].texCoords = sf::Vector2f(
			static_cast<f32>(tileX) * s.x,
			static_cast<f32>(tileY+1) * s.y
	);
}

void Tilemap::setTileColor(s32 x, s32 y, const sf::Color& color)
{
	sf::Vertex * quad = &m_vertices[(y * m_size.x + x) * 4];

	quad[0].color = color;
	quad[1].color = color;
	quad[2].color = color;
	quad[3].color = color;
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the tileset texture
	states.texture = r_atlas;

	// draw the vertex array
	target.draw(m_vertices, states);
}

} // namespace zn




