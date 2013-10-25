/*
AssetBank.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <fstream>
#include <iostream>
#include <cstdio>
#include "AssetBank.hpp"
#include "../rapidjson/document.h"
#include "../rapidjson/filestream.h"

using namespace rapidjson;

namespace zn
{

AssetBank * g_currentRef = nullptr;

AssetBank::AssetBank()
{
	if(g_currentRef == nullptr)
		makeCurrent();
}

AssetBank::~AssetBank()
{
	if(g_currentRef == this)
		g_currentRef = nullptr;
}

//------------------------------------------------------------------------------
void AssetBank::setRootFolder(const std::string & rf)
{
#ifdef ZN_DEBUG
	std::cout << "D: AssetBank: root folder set to " << rf << '.' << std::endl;
#endif // ZN_DEBUG
	fonts.setRootFolder(rf);
	textures.setRootFolder(rf);
	//atlases.setRootFolder(rf);
	//maps.setRootFolder(rf);
}

//------------------------------------------------------------------------------

bool AssetBank::loadFromJSON(const std::string & filePath)
{
	FILE * cfile = fopen(filePath.c_str(), "rb");
	if(!cfile)
	{
		std::cout << "E: AssetBank::loadFromJSON: couldn't open \"" + filePath + '"' << std::endl;
		return false;
	}

	std::cout << "Reading AssetBank " << filePath << "..." << std::endl;

	// Parse stream

	FileStream fs(cfile);
	Document doc;
	doc.ParseStream<0>(fs);

	// Load assets

	if(doc.HasMember("textures"))
	{
		if(!textures.loadList(doc["textures"]))
			return false;
	}

	if(doc.HasMember("fonts"))
	{
		if(!fonts.loadList(doc["fonts"]))
			return false;
	}

	if(doc.HasMember("soundbuffers"))
	{
		if(!soundBuffers.loadList(doc["soundbuffers"]))
			return false;
	}

	if(doc.HasMember("soundstreams"))
	{
		if(!soundStreams.loadList(doc["soundstreams"]))
			return false;
	}

	if(doc.HasMember("atlases"))
	{
		if(!atlases.loadList(doc["atlases"]))
			return false;
	}

	if(doc.HasMember("maps"))
	{
		if(!maps.loadList(doc["maps"]))
			return false;
	}

	fclose(cfile);

	std::cout << "Done" << std::endl;

	return true;
}


//------------------------------------------------------------------------------
/*
bool AssetBank::loadFolder(const std::string & folderPath)
{
	std::cout << "AssetBank::loadFolder: not implemented yet !" << std::endl;
	// TODO implement
	return false;
}
*/

//------------------------------------------------------------------------------
void AssetBank::makeCurrent()
{
	g_currentRef = this;
}

//------------------------------------------------------------------------------
AssetBank * AssetBank::current()
{
	return g_currentRef;
}

} // namespace zn


