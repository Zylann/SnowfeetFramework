/*
TiledMap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_TILEDMAP_H_INCLUDED
#define HEADER_ZN_TILEDMAP_H_INCLUDED

#include <SFML/System.hpp>
#include <string>
#include <vector>
#include "../types.hpp"
#include "../util/Exception.hpp"

namespace zn
{

// A Tiled importer that uses rapidjson to parse a JSON map.
// Note: not all features of Tiled are used here.
// Tiled is a free and open-source 2D map editor.
// http://www.mapeditor.org/
class ZN_API TiledMap
{
public:

	// ----------------------
	// Structures
	//-----------------------

	struct Object
	{
		u32               gid;
		std::string       name;
		std::string       type;
		bool              visible;
		sf::Vector2i      position;
		sf::Vector2i      size;

		Object() : gid(0), visible(false), position(0,0) {}
	};

	struct Layer
	{
		sf::Vector2i         size;
		sf::Vector2i         position;
		std::string          name;
		std::vector<u32>     grid;
		std::vector<Object>  objects;

		Layer() : size(0,0), position(0,0) {}

		inline u32 at(u32 x, u32 y) const { return grid[y*size.x + x]; }

		const Object * objectFromName(const std::string & obName) const
		{
			for(u32 i = 0; i < objects.size(); ++i)
			{
				if(objects[i].name == obName)
					return &objects[i];
			}
			return nullptr;
		}
	};

	struct TileSet
	{
		std::string       name;
		u32               firstgid;
		sf::Vector2i      tileSize;
		std::string       texturePath;

		TileSet() : firstgid(0), tileSize(0,0), texturePath("") {}
	};

	//-----------------------
	// Methods
	//-----------------------

	bool loadFromFile(const std::string & filePath);
	bool loadFromJSONFile(const std::string & filePath);

	void print(std::ostream & os) const;

	const Layer * layerFromName(const std::string & layerName) const;
	const TileSet * tileSetFromName(const std::string & tileSetName) const;

	//-----------------------
	// Attributes
	//-----------------------

	sf::Vector2i          size;
	sf::Vector2i          tileSize;
	std::vector<Layer>    layers;
	std::vector<TileSet>  tileSets;

};

} // namespace zn

#endif // HEADER_ZN_TILEDMAP_H_INCLUDED

