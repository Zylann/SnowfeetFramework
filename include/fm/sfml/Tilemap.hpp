/*
Tilemap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_TILEMAP_H_INCLUDED
#define HEADER_ZN_TILEMAP_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <fm/util/Vector2.hpp>

namespace zn
{

class ZN_API Tilemap : public sf::Drawable
{
public:

	Tilemap() :
		r_atlas(nullptr),
		m_tileSize(32)
	{}

	void create(u32 width, u32 height);

	void setAtlas(const sf::Texture * atlas, u32 tileSize);
	void setAtlas(const sf::Texture * atlas, Vector2i tileSize);

	void setTile(s32 x, s32 y, u32 tile);
	void setTile(s32 x, s32 y, u32 tileX, u32 tileY);
	void setTileColor(s32 x, s32 y, const sf::Color & color);

	bool contains(s32 x, s32 y);

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	// TODO divide into chunks?

	sf::VertexArray m_vertices;
	const sf::Texture * r_atlas;
	Vector2i m_size;
	Vector2i m_tileSize;
//	Vector2f m_normalizedTileSize;

};

} // namespace zn

#endif // HEADER_ZN_TILEMAP_H_INCLUDED


