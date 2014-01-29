/*
AssetBank.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <fstream>
#include <iostream>
#include <cstdio>

#include <fm/asset/AssetBank.hpp>

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
void AssetBank::setRootFolder(const std::string & rootFolder)
{
#ifdef ZN_DEBUG
	std::cout << "D: AssetBank: root folder set to " << rootFolder << '.' << std::endl;
#endif // ZN_DEBUG
	m_root = rootFolder;
}

//------------------------------------------------------------------------------
bool AssetBank::loadFromJSON(const std::string & manifestPath)
{
	std::ifstream ifs(manifestPath.c_str(), std::ios::in|std::ios::binary);
	if(!ifs.good())
	{
		std::cout << "E: AssetBank::loadFromJSON: couldn't open \"" + manifestPath + '"' << std::endl;
		return false;
	}

	std::cout << "I: Reading AssetBank " << manifestPath << "..." << std::endl;

	// Parse stream

	JsonBox::Value doc;
	doc.loadFromStream(ifs);
	ifs.close();

	// Load assets

	// TODO make this more generic
	// TODO add loading parameters such as lazy loading (load a manifest instead of directly read the file)

	// Note: the loading order is important

	if(!doc["textures"].isNull())
	{
		if(!textures.loadList(doc["textures"], m_root))
			return false;
	}

	if(!doc["shaders"].isNull())
	{
		if(!shaders.loadList(doc["shaders"], m_root))
			return false;
	}

	if(!doc["fonts"].isNull())
	{
		if(!fonts.loadList(doc["fonts"], m_root))
			return false;
	}

	if(!doc["soundbuffers"].isNull())
	{
		if(!soundBuffers.loadList(doc["soundbuffers"], m_root))
			return false;
	}

	if(!doc["soundstreams"].isNull())
	{
		if(!soundStreams.loadList(doc["soundstreams"], m_root))
			return false;
	}

	// Note: materials might depend on textures
	if(!doc["materials"].isNull())
	{
		if(!materials.loadList(doc["materials"], m_root))
			return false;
	}

	// Note: atlases might depend on textures/materials
	if(!doc["atlases"].isNull())
	{
		if(!atlases.loadList(doc["atlases"], m_root))
			return false;
	}

	// Note: maps might depend on textures or atlases
	if(!doc["maps"].isNull())
	{
		if(!maps.loadList(doc["maps"], m_root))
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


