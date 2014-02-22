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

	m_assetMaps.push_back(&textures);
	m_assetMaps.push_back(&shaders);
	m_assetMaps.push_back(&fonts);
	m_assetMaps.push_back(&atlases);
	m_assetMaps.push_back(&maps);
	m_assetMaps.push_back(&soundBuffers);
	m_assetMaps.push_back(&soundStreams);
	m_assetMaps.push_back(&materials);
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
bool AssetBank::loadFileAssociations(const std::string & assocFile, bool create)
{
	std::cout << "I: Loading file associations..." << std::endl;

	JsonBox::Value doc;
	if(!loadFromFile(doc, assocFile, -1, !create))
	{
		// The file couldn't be opened

		if(create)
		{
			std::cout << "I: File associations file not found. Creating a new one." << std::endl;

			// Create default associations

			// Textures
			JsonBox::Array exts;
			exts.push_back("png");
			exts.push_back("jpg");
			exts.push_back("jpeg");
			doc[textures.tag] = exts;

			// Shaders
			exts.clear();
			exts.push_back("shader");
			doc[shaders.tag] = exts;

			// Fonts
			exts.clear();
			exts.push_back("ttf");
			doc[fonts.tag] = exts;

			// Sound buffers
			exts.clear();
			exts.push_back("wav");
			exts.push_back("ogg");
			doc[soundBuffers.tag] = exts;

			// Sound streams
			exts.clear();
			exts.push_back("wav");
			exts.push_back("ogg");
			doc[soundStreams.tag] = exts;

			// Save file
			std::ofstream ofs(assocFile, std::ios::out|std::ios::binary|std::ios::trunc);
			if(ofs.good())
			{
				doc.writeToStream(ofs);
				ofs.close();
			}
			else
			{
				std::cout << "E: AssetBank::loadFileAssociations: "
					"couldn't write file." << std::endl;
			}
		}
		else
		{
			// File not found and no create option, so error return
			return false;
		}
	}

	// Load file associations
	for(auto it = m_assetMaps.begin(); it != m_assetMaps.end(); ++it)
	{
		AssetMapBase & m = **it;
		m.loadFileAssociations(doc[m.tag]);
	}

	return true;
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
	// Warning: the loading order is important

	if(!textures.loadManifestGroup(doc, m_root)) return false;
	if(!shaders.loadManifestGroup(doc, m_root)) return false;
	if(!fonts.loadManifestGroup(doc, m_root)) return false;
	if(!soundBuffers.loadManifestGroup(doc, m_root)) return false;
	if(!soundStreams.loadManifestGroup(doc, m_root)) return false;

	// Note: materials might depend on textures
	if(!materials.loadManifestGroup(doc, m_root)) return false;

	// Note: atlases might depend on textures/materials
	if(!atlases.loadManifestGroup(doc, m_root)) return false;

	// Note: maps might depend on textures or atlases
	if(!maps.loadManifestGroup(doc, m_root)) return false;

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


