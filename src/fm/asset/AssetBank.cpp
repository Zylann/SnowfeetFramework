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
#include <fm/util/Log.hpp>

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
	materials("material"),
	m_manifestFileName("manifest.json"),
	m_assocFileName("file_associations.json")
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
bool AssetBank::load(const std::string & rootFolder, bool performAutoIndex)
{
	setRootFolder(rootFolder);

	// Load or create file associations file
	if(!loadFileAssociations(m_root + '/' + m_assocFileName, true))
	{
		return false;
	}

	std::string manifestPath = m_root + '/' + m_manifestFileName;

	AssetBankManifest manifest;
	bool foundManifest = manifest.loadFromJSONFile(manifestPath);

	if(performAutoIndex)
	{
		AssetBankManifest automaticManifest;

		autoIndex(m_root, automaticManifest);

		manifest.merge(automaticManifest, false);
	}

	// If there was no manifest file
	if(!foundManifest)
	{
		// Create it
		manifest.saveToJSONFile(manifestPath);
	}

	// TODO asynchronous loading of assets with a progressBar

	// Effectively load assets
	if(!loadAssets(manifest))
	{
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
bool AssetBank::autoIndex(const std::string & folderPath, AssetBankManifest & manifest)
{
	log.info() << "Indexing folder \"" << folderPath << '"' << log.endl();

	// Get file list recursively
	std::vector<std::string> files;
	if(!getFiles(folderPath, files, true))
	{
		log.err() << "AssetBank: folder indexing failed." << log.endl();
		return false;
	}

	std::vector<std::string> unknownFiles;

	// For each file
	for(auto it = files.begin(); it != files.end(); ++it)
	{
		std::string path = *it;

		// Remove root folder from the path (including the end '/')
		path = path.substr(folderPath.size()+1);

		// Should the file be ignored?
		if(m_ignoredFilesMatcher.evaluate(path))
		{
			// Skip the file
			continue;
		}

		const AssetMapBase * assetMap = findAssetType(path);

		if(assetMap != nullptr)
		{
			std::string name = fileNameWithoutExtension(path);
			auto & entries = manifest.sections[assetMap->manifestTag()];

			// Search if the name is already in use for this type of asset
			auto it = entries.find(name);
			if(it != entries.end())
			{
				// Duplicate, revert the name to a full path
				log.warn() << "Auto-indexing found two assets with the same name : \""
					<< it->second << "\" and \"" << path << "\"."
					"Full path will be used for duplicates." << log.endl();
				name = path;
			}

			entries[name] = path;
		}
		else
		{
			unknownFiles.push_back(path);
		}
	}

	if(!unknownFiles.empty())
	{
		log.warn() << "Some asset files were not associated by the auto-indexing process. "
			"You might add them manually to the manifest, or modify file associations." << log.endl();

		log.more() << "--- Concerned files: ---" << log.endl();

		for(auto it = unknownFiles.begin(); it != unknownFiles.end(); ++it)
		{
			log.more() << (*it) << log.endl();
		}

		log.more() << "---" << log.endl();
	}

	return true;
}

//------------------------------------------------------------------------------
const AssetMapBase * AssetBank::findAssetType(const std::string & filePath)
{
	// Search for a matching type
	std::vector<AssetMapBase*> matchingTypes;
	for(auto it = m_assetMaps.begin(); it != m_assetMaps.end(); ++it)
	{
		AssetMapBase & assetMap = **it;
		if(assetMap.matcher.evaluate(filePath))
		{
			matchingTypes.push_back(&assetMap);
		}
	}

	// If only one type matches the extension
	if(matchingTypes.size() == 1)
	{
		// Return it
		return *(matchingTypes.begin());
	}

	// Otherwise, it's ambiguous.
	return nullptr;
}

//------------------------------------------------------------------------------
void AssetBank::setRootFolder(const std::string & rootFolder)
{
#ifdef ZN_DEBUG
	log.debug() << "AssetBank: root folder set to " << rootFolder << '.' << log.endl();
#endif // ZN_DEBUG

	m_root = rootFolder;

	for(auto it = m_assetMaps.begin(); it != m_assetMaps.end(); ++it)
	{
		(*it)->setRootFolder(m_root);
	}
}

//------------------------------------------------------------------------------
bool AssetBank::loadFileAssociations(const std::string & assocFile, bool create)
{
	log.info() << "Loading file associations..." << log.endl();

	JsonBox::Value doc;
	if(!loadFromFile(doc, assocFile, -1, !create))
	{
		// The file couldn't be opened

		if(create)
		{
			log.info() << "File associations file not found. Creating a new one." << log.endl();

			// Create default associations
			createDefaultFileAssociations(doc);

			// Save file
			std::ofstream ofs(assocFile, std::ios::out|std::ios::binary|std::ios::trunc);
			if(ofs.good())
			{
				doc.writeToStream(ofs);
				ofs.close();
			}
			else
			{
				log.err() << "AssetBank::loadFileAssociations: "
					"couldn't write file." << log.endl();
			}
		}
		else
		{
			// File not found and no create option, so error return
			return false;
		}
	}

	// Load ignore matcher
	m_ignoredFilesMatcher.loadFromJSON(doc["ignore"]);
	m_ignoredFilesMatcher.addPattern(m_manifestFileName);
	m_ignoredFilesMatcher.addPattern(m_assocFileName);

	// Load file associations
	for(auto it = m_assetMaps.begin(); it != m_assetMaps.end(); ++it)
	{
		AssetMapBase & m = **it;
		m.loadFileAssociations(doc[m.tag]);
	}

	return true;
}

//------------------------------------------------------------------------------
void AssetBank::createDefaultFileAssociations(JsonBox::Value & doc)
{
	// Textures
	JsonBox::Array patterns;
	patterns.push_back("*.png");
	patterns.push_back("*.jpg");
	patterns.push_back("*.jpeg");
	doc[textures.tag] = patterns;

	// Shaders
	patterns.clear();
	patterns.push_back("*.shader");
	doc[shaders.tag] = patterns;

	// Fonts
	patterns.clear();
	patterns.push_back("*.ttf");
	doc[fonts.tag] = patterns;

	// Sound buffers
	patterns.clear();
	patterns.push_back("sounds/*.wav");
	patterns.push_back("sounds/*.ogg");
	doc[soundBuffers.tag] = patterns;

	// Sound streams
	patterns.clear();
	patterns.push_back("music/*.wav");
	patterns.push_back("music/*.ogg");
	doc[soundStreams.tag] = patterns;

	// Maps
	patterns.clear();
	patterns.push_back("maps/*.json");
	doc[maps.tag] = patterns;

	// Atlases
	patterns.clear();
	patterns.push_back("atlases/*.json");
	patterns.push_back("*.atlas");
	doc[atlases.tag] = patterns;

	// Materials
	patterns.clear();
	patterns.push_back("materials/*.json");
	doc[materials.tag] = patterns;

	// Ignore
	patterns.clear();
	patterns.push_back("(*/_*)|(_*)");
	doc["ignore"] = patterns;
}

//------------------------------------------------------------------------------
bool AssetBank::loadAssets(AssetBankManifest & manifest)
{
	// Load assets
	// TODO add loading parameters such as lazy loading (load a manifest instead of directly read the file)
	// Warning: the loading order is important

	if(!textures.loadManifestGroup(manifest, m_root)) return false;
	if(!shaders.loadManifestGroup(manifest, m_root)) return false;
	if(!fonts.loadManifestGroup(manifest, m_root)) return false;
	if(!soundBuffers.loadManifestGroup(manifest, m_root)) return false;
	if(!soundStreams.loadManifestGroup(manifest, m_root)) return false;

	// Note: materials might depend on textures
	if(!materials.loadManifestGroup(manifest, m_root)) return false;

	// Note: atlases might depend on textures/materials
	if(!atlases.loadManifestGroup(manifest, m_root)) return false;

	// Note: maps might depend on textures or atlases
	if(!maps.loadManifestGroup(manifest, m_root)) return false;

	log.info() << "Done" << log.endl();

	return true;
}


} // namespace zn


