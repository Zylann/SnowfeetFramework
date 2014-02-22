/*
AssetBank.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <fstream>
#include <iostream>
#include <cstdio>

#include <fm/asset/AssetBank.hpp>
#include <fm/system/filesystem.hpp>

namespace zn
{

AssetBank * g_currentRef = nullptr;

//------------------------------------------------------------------------------
AssetBank::AssetBank() :
	textures("texture"),
	shaders("shader"),
	fonts("font"),
	atlases("atlas"),
	maps("map"),
	soundBuffers("soundbuffer"),
	soundStreams("soundstream"),
	materials("material")
{
	if(g_currentRef == nullptr)
	{
		makeCurrent();
	}
}

//------------------------------------------------------------------------------
AssetBank::~AssetBank()
{
	if(g_currentRef == this)
	{
		g_currentRef = nullptr;
	}
}

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
	// TODO add loading parameters such as lazy loading (load a manifest instead of directly read the file)
	// Note: the loading order is important

	if(!loadManifestGroup(doc, textures)) return false;
	if(!loadManifestGroup(doc, shaders)) return false;
	if(!loadManifestGroup(doc, fonts)) return false;
	if(!loadManifestGroup(doc, soundBuffers)) return false;
	if(!loadManifestGroup(doc, soundStreams)) return false;

	// Note: materials might depend on textures
	if(!loadManifestGroup(doc, materials)) return false;

	// Note: atlases might depend on textures/materials
	if(!loadManifestGroup(doc, atlases)) return false;

	// Note: maps might depend on textures or atlases
	if(!loadManifestGroup(doc, maps)) return false;

	std::cout << "I: Done" << std::endl;

	return true;
}


//------------------------------------------------------------------------------
//bool AssetBank::loadFolder(const std::string & folderPath)
//{
//	std::cout << "I: Loading folder \"" << folderPath << '"' << std::endl;
//
//	std::vector<std::string> files;
//	if(!getFiles(folderPath, files, true))
//	{
//		std::cout << "E: AssetBank::loadFolder: an error occurred." << std::endl;
//		return false;
//	}
//
//  WIP
//
//	return false;
//}

} // namespace zn


