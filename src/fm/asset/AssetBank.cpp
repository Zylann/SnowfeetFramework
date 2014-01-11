/*
AssetBank.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <fstream>
#include <iostream>
#include <cstdio>
#include "AssetBank.hpp"

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
	std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
	if(!ifs.good())
	{
		std::cout << "E: AssetBank::loadFromJSON: couldn't open \"" + filePath + '"' << std::endl;
		return false;
	}

	std::cout << "I: Reading AssetBank " << filePath << "..." << std::endl;

	// Parse stream

	JsonBox::Value doc;
	doc.loadFromStream(ifs);
	ifs.close();

	// Load assets

	// TODO make this more generic
	// TODO add loading parameters such as lazy loading (load a manifest instead of directly read the file)

	if(!doc["textures"].isNull())
	{
		if(!textures.loadList(doc["textures"]))
			return false;
	}

	if(!doc["fonts"].isNull())
	{
		if(!fonts.loadList(doc["fonts"]))
			return false;
	}

	if(!doc["soundbuffers"].isNull())
	{
		if(!soundBuffers.loadList(doc["soundbuffers"]))
			return false;
	}

	if(!doc["soundstreams"].isNull())
	{
		if(!soundStreams.loadList(doc["soundstreams"]))
			return false;
	}

	if(!doc["atlases"].isNull())
	{
		if(!atlases.loadList(doc["atlases"]))
			return false;
	}

	if(!doc["maps"].isNull())
	{
		if(!maps.loadList(doc["maps"]))
			return false;
	}

	std::cout << "I: Done" << std::endl;

	return true;
}


//------------------------------------------------------------------------------
/*
bool AssetBank::loadFolder(const std::string & folderPath)
{
	std::cout << "AssetBank::loadFolder: not implemented yet !" << std::endl;
	// TODO implement AssetBank::loadFolder()
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


