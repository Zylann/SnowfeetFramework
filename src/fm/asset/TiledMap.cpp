/*
TiledMap.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <iostream>
#include <cstdio>
#include <sstream>
#include "../util/Exception.hpp"
#include "../util/stringutils.hpp"
#include "../rapidjson/document.h"
#include "../rapidjson/filestream.h"
#include "TiledMap.hpp"

#define xassert(__test)                                              \
	if(!__test) {                                                    \
		std::stringstream ss;                                        \
		ss << __FILE__ << ", line " << __LINE__ << ": " << #__test;  \
		throw Exception(ss.str());                                   \
	}

using namespace rapidjson;

namespace zn
{

//------------------------------------------------------------------------------
bool TiledMap::loadFromFile(const std::string & filePath)
{
	try
	{
		loadFromJSONFile(filePath);
	}
	catch(std::exception & e)
	{
		std::cout << "E: TiledMap::loadFromFile: assertion failed: " << e.what() << std::endl;
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
// Helper function
sf::Vector2i readSize(const Value & v, const char * wname, const char * hname)
{
	sf::Vector2i s;

	xassert(v.HasMember(wname));
	xassert(v[wname].IsUint())
	s.x = v[wname].GetUint();

	xassert(v.HasMember(hname));
	xassert(v[hname].IsUint())
	s.y = v[hname].GetUint();

	return s;
}

//------------------------------------------------------------------------------
// Helper function
sf::Vector2i readPosition(const Value & v, const char * xname, const char * yname)
{
	sf::Vector2i s;

	xassert(v.HasMember(xname));
	xassert(v[xname].IsInt())
	s.x = v[xname].GetInt();

	xassert(v.HasMember(yname));
	xassert(v[yname].IsInt())
	s.y = v[yname].GetInt();

	return s;
}

//------------------------------------------------------------------------------
// Helper function
std::string readString(const Value & v, const char * name)
{
	std::string s;

	xassert(v.HasMember(name));
	xassert(v[name].IsString());
	s = v[name].GetString();

	return s;
}

//------------------------------------------------------------------------------
void TiledMap::loadFromJSONFile(const std::string & filePath) throw(Exception)
{
	// TODO TiledMap: cleaner code
	// A lot of lines are for testing and reporting errors.
	// It would be cleaner if no check were made : fail quick and hard.
	// Then, the debugger should be a good friend here.

	FILE * cfile = fopen(filePath.c_str(), "rb");
	if(!cfile)
	{
		throw Exception("E: TiledMap::loadFromJSONFile: couldn't open \"" + filePath + '"');
	}

	FileStream fs(cfile);

	//
	// Root
	//

	std::cout << "I: Reading TiledMap JSON..." << std::endl;

	Document doc;

	doc.ParseStream<0>(fs);

	xassert(doc.IsObject());

	// Check JSON version
	s32 version = doc["version"].GetInt();
	if(version != 1)
	{
		fclose(cfile);
		throw Exception("E: TiledMap: unsupported JSON version (" + toString(version) + ")");
	}

	// Global size
	size = readSize(doc, "width", "height");

	// Global tile size
	tileSize = readSize(doc, "tilewidth", "tileheight");

	//
	// Layers
	//

	xassert(doc.HasMember("layers"));
	const Value & jlayers = doc["layers"];
	xassert(jlayers.IsArray());

	layers.resize(jlayers.Size(), Layer());

	// Fetch layers
	for(SizeType layer_i = 0; layer_i < jlayers.Size(); ++layer_i)
	{
		// Get layer
		const Value & jlayer = jlayers[layer_i];
		xassert(jlayer.IsObject());

		Layer & layer = layers[layer_i];

		// Read layer information
		layer.size     = readSize(jlayer, "width", "height");
		layer.position = readPosition(jlayer, "x", "y");
		layer.name     = readString(jlayer, "name");

		std::string layerType = readString(jlayer, "type");

		if(layerType == "tilelayer")
		{
			std::cout << "I: Reading tilelayer..." << std::endl;

			// Get data
			xassert(jlayer.HasMember("data"));
			const Value & jdata = jlayer["data"];
			xassert(jdata.IsArray());

			// Check if dimensions are consistent
			u32 layerDataSize = layer.size.x*layer.size.y;
			if(layerDataSize != jdata.Size())
			{
				fclose(cfile);
				throw(Exception("layerDataSize != width*height (layer name: \"" + layer.name + "\")"));
			}

			// Read data
			layer.grid.resize(layerDataSize);
			for(SizeType i = 0; i < jdata.Size(); ++i)
			{
				layer.grid[i] = jdata[i].GetUint();
			}
		}
		else if(layerType == "objectgroup")
		{
			std::cout << "I: Reading objectgroup..." << std::endl;

			// Get data
			const Value & jobjects = jlayer["objects"];

			// Read data
			layer.objects.resize(jobjects.Size());
			for(SizeType i = 0; i < jobjects.Size(); ++i)
			{
				const Value & jobj = jobjects[i];
				Object & obj = layer.objects[i];

				obj.position.x  = jobj["x"].GetInt();
				obj.position.y  = jobj["y"].GetInt();
				obj.gid         = jobj["gid"].GetInt();
				obj.visible     = jobj["visible"].GetBool();
				obj.size.x      = jobj["width"].GetInt();
				obj.size.y      = jobj["height"].GetInt();
			}
		}
	}

	//
	// Tilesets
	//

	xassert(doc.HasMember("tilesets"));
	const Value & jtilesets = doc["tilesets"];
	xassert(jtilesets.IsArray());

	tileSets.resize(jtilesets.Size(), TileSet());

	for(SizeType i = 0; i < jtilesets.Size(); ++i)
	{
		std::cout << "I: Reading tileset " << i << "..." << std::endl;

		const Value & jtileset = jtilesets[i];
		xassert(jtileset.IsObject());

		TileSet & tileset = tileSets[i];

		// Tileset size
		tileset.tileSize = readSize(jtileset, "tilewidth", "tileheight");
		// Tileset name
		tileset.name = readString(jtileset, "name");

		// firstgid
		xassert(jtileset.HasMember("firstgid"));
		xassert(jtileset["firstgid"].IsUint());
		tileset.firstgid = jtileset["firstgid"].GetUint();

		// texture path
		tileset.texturePath = readString(jtileset, "image");
	}

	fclose(cfile);
}

//------------------------------------------------------------------------------
void TiledMap::print(std::ostream & os) const
{
	using namespace std;

	cout << "----- TiledMap -----" << endl;

	cout << "Global size: " << size.x << "x" << size.y << endl;
	cout << "Global tile size: " << tileSize.x << "x" << tileSize.y << endl;
	cout << layers.size() << " layers:" << endl;
	for(u32 i = 0; i < layers.size(); ++i)
	{
		const Layer & l = layers[i];
		cout << "  Layer " << i << ":" << endl;
		cout << "    name: " << l.name << ", " << endl
			 << "    size: " << l.size.x << "x" << l.size.y << ", " << endl
		 	 << "    position: (" << l.position.x << ", " << l.position.y << ")" << endl
		 	 << "    objects: " << l.objects.size() << endl;
	}
	cout << tileSets.size() << " tilesets: " << endl;
	for(u32 i = 0; i < tileSets.size(); ++i)
	{
		const TileSet & ts = tileSets[i];
		cout << "  Tileset " << i << ":" << endl;
		cout << "    name: " << ts.name << ", " << endl
			 << "    texturePath: " << ts.texturePath << ", " << endl
			 << "    firstgid: " << ts.firstgid << ", " << endl
			 << "    tileSize: " << ts.tileSize.x << "x" << ts.tileSize.y << endl;
	}

	cout << "--------------------" << endl;
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







