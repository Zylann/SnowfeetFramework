/*
TiledMap.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <iostream>
#include <cstdio>
#include <sstream>
#include <cassert>
#include <fstream>

#include <fm/util/Exception.hpp>
#include <fm/util/stringutils.hpp>
#include <fm/json/json_utils.hpp>
#include <fm/asset/TiledMap.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

//------------------------------------------------------------------------------
bool TiledMap::loadFromFile(const std::string & filePath)
{
	return loadFromJSONFile(filePath);
}

//------------------------------------------------------------------------------
bool TiledMap::loadFromJSONFile(const std::string & filePath)
{
	// Parse stream

	log.debug() << "Reading TiledMap JSON..." << log.endl();
	JsonBox::Value doc;
	if(!zn::loadFromFile(doc, filePath, 1))
	{
		return false;
	}

	//
	// Globals
	//

	// Map size
	size.x = doc["width"].getInt();
	size.y = doc["height"].getInt();

	// Tiles size
	tileSize.x = doc["tilewidth"].getInt();
	tileSize.y = doc["tileheight"].getInt();

	//
	// Layers
	//

	JsonBox::Array jlayers = doc["layers"].getArray();
	layers.resize(jlayers.size(), Layer());

	// Fetch layers
	u32 layerIndex = 0;
	for(auto layerIt = jlayers.begin(); layerIt != jlayers.end(); ++layerIt, ++layerIndex)
	{
		Layer & layer = layers[layerIndex];

		// Read layer information
		layer.size.x      = (*layerIt)["width"].getInt();
		layer.size.y      = (*layerIt)["height"].getInt();
		layer.position.x  = (*layerIt)["x"].getInt();
		layer.position.y  = (*layerIt)["y"].getInt();
		layer.name        = (*layerIt)["name"].getString();

		std::string layerType = (*layerIt)["type"].getString();

		if(layerType == "tilelayer")
		{
			log.debug() << "Reading tilelayer..." << log.endl();

			// Get tiles data
			JsonBox::Array jdata = (*layerIt)["data"].getArray();

			// Check if dimensions are consistent
			u32 layerDataSize = layer.size.x * layer.size.y;
			if(layerDataSize != jdata.size())
			{
				// Dimensions mismatch !
				log.err() << "jdata.size()(" << jdata.size() << ")"
					" != width(" << layer.size.x << ")*height(" << layer.size.y << ")"
					" (layer name: \"" << layer.name << "\")" << log.endl();
				return false;
			}

			// Read tiles data
			layer.grid.resize(layerDataSize);
			u32 i = 0;
			for(auto it = jdata.begin(); it != jdata.end(); ++it)
			{
				layer.grid[i] = it->getInt();
				++i;
			}
		}
		else if(layerType == "objectgroup")
		{
			log.debug() << "Reading objectgroup..." << log.endl();

			// Get objects data
			JsonBox::Array jobjects = (*layerIt)["objects"].getArray();

			// Read data
			layer.objects.resize(jobjects.size());
			u32 i = 0;
			for(auto it = jobjects.begin(); it != jobjects.end(); ++it)
			{
				JsonBox::Object jobj = it->getObject();
				Object & obj = layer.objects[i];

				obj.position.x  = jobj["x"].getInt();
				obj.position.y  = jobj["y"].getInt();
				obj.gid         = jobj["gid"].getInt();
				obj.visible     = jobj["visible"].getBoolean();
				obj.size.x      = jobj["width"].getInt();
				obj.size.y      = jobj["height"].getInt();
			}
		}
	}

	//
	// Tilesets
	//

	JsonBox::Array jtilesets = doc["tilesets"].getArray();

	tileSets.resize(jtilesets.size(), TileSet());

	u32 tilesetIndex = 0;
	for(auto tilesetIt = jtilesets.begin(); tilesetIt != jtilesets.end(); ++tilesetIt, ++tilesetIndex)
	{
		log.debug() << "Reading tileset " << tilesetIndex << "..." << log.endl();

		TileSet & tileset = tileSets[tilesetIndex];

		// Tileset size
		tileset.tileSize.x = (*tilesetIt)["tilewidth"].getInt();
		tileset.tileSize.y = (*tilesetIt)["tileheight"].getInt();

		// Tileset name
		tileset.name = (*tilesetIt)["name"].getString();

		// Firstgid
		tileset.firstgid = (*tilesetIt)["firstgid"].getInt();

		// Texture path
		tileset.texturePath = (*tilesetIt)["image"].getString();
	}

	//print(std::cout);

	return true;
}

//------------------------------------------------------------------------------
void TiledMap::print(std::ostream & os) const
{
	using namespace std;

	os << "----- TiledMap -----" << endl;

	os << "Global size: " << size.x << "x" << size.y << endl;
	os << "Global tile size: " << tileSize.x << "x" << tileSize.y << endl;
	os << layers.size() << " layers:" << endl;
	for(u32 i = 0; i < layers.size(); ++i)
	{
		const Layer & l = layers[i];
		os << "  Layer " << i << ":" << endl;
		os << "    name: " << l.name << ", " << endl
		   << "    size: " << l.size.x << "x" << l.size.y << ", " << endl
		   << "    position: (" << l.position.x << ", " << l.position.y << ")" << endl
		   << "    objects: " << l.objects.size() << endl;
	}
	os << tileSets.size() << " tilesets: " << endl;
	for(u32 i = 0; i < tileSets.size(); ++i)
	{
		const TileSet & ts = tileSets[i];
		os << "  Tileset " << i << ":" << endl;
		os << "    name: " << ts.name << ", " << endl
		   << "    texturePath: " << ts.texturePath << ", " << endl
		   << "    firstgid: " << ts.firstgid << ", " << endl
		   << "    tileSize: " << ts.tileSize.x << "x" << ts.tileSize.y << endl;
	}

	os << "--------------------" << endl;
}

//------------------------------------------------------------------------------
const TiledMap::Layer * TiledMap::layerFromName(const std::string & layerName) const
{
	for(u32 i = 0; i < layers.size(); ++i)
	{
		if(layers[i].name == layerName)
			return &layers[i];
	}
	return nullptr;
}

//------------------------------------------------------------------------------
const TiledMap::TileSet * TiledMap::tileSetFromName(const std::string & tileSetName) const
{
	for(u32 i = 0; i < tileSets.size(); ++i)
	{
		if(tileSets[i].name == tileSetName)
			return &tileSets[i];
	}
	return nullptr;
}

} // namespace zn







