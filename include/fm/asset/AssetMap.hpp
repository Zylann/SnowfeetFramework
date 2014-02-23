/*
AssetMap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_ASSETMAP_HPP_INCLUDED
#define HEADER_ZN_ASSETMAP_HPP_INCLUDED

#include <unordered_map>
#include <iostream>

#include <fm/util/stringutils.hpp>
#include <fm/json/json_utils.hpp>
#include <fm/asset/load.hpp>
#include <fm/asset/AssetMapBase.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

/// \brief This generic class handles the loading of specific-type assets and indexes them in a map.
/// assets with the same file path are not reloaded.
/// The template parameter must be class where instances can be loaded and possibly
/// saved to a file. To be compatible, the class must have a dedicated function overload
/// in load.hpp/cpp.
/// \see load.hpp
template <class T>
class AssetMap : public AssetMapBase
{
public:

	AssetMap(std::string pTag) : AssetMapBase(pTag)
	{}

	~AssetMap()
	{
		clear();
	};

	u32 size() const override
	{
		return m_map.size();
	}

	/// \brief Releases all assets from this map. This effectively deletes the assets,
	/// you must be absolutely sure that no one is in use when doing that.
	void clear()
	{
		for(auto it = m_map.begin(); it != m_map.end(); ++it)
			delete it->second;
		m_map.clear();
	}

	// Gets an asset from its name
	T * get(const std::string & id) const
	{
		auto it = m_map.find(id);
		if(it != m_map.end())
			return it->second;
		else
		{
#ifdef ZN_DEBUG
			log.err() << "AssetMap::get: not found \"" << id << '"' << log.endl();
#endif
			return nullptr;
		}
	}

	// Finds the name of an asset from its pointer.
	// Returns an empty string if the asset is not contained in the map.
	std::string findName(const T * asset) const
	{
		for(auto it = m_map.cbegin(); it != m_map.cend(); ++it)
		{
			if(it->second == asset)
				return it->first;
		}
		return "";
	}

	// TODO ability to create assets managed by AssetMap at runtime

	// Loads an asset from a file into the map with a name
	bool load(std::string filePath, const std::string & name)
	{
		if(name.empty())
		{
#ifdef ZN_DEBUG
			log.err() << "AssetMap::load: empty name specified !" << log.endl();
#endif
			return false;
		}

		if(m_map.find(name) != m_map.end())
			return true; // Already loaded

		if(!m_rootFolder.empty())
		{
			filePath = m_rootFolder + '/' + filePath;
		}

#ifdef ZN_DEBUG
		log.debug() << "AssetMap: loading \"" << filePath << "\" as \"" << name << '"' << log.endl();
#endif

		T * asset = new T();
		if(!loadAsset(asset, filePath))
		{
			delete asset;
			return false;
		}
		else
		{
			m_map[name] = asset;
			return true;
		}
	}

	bool loadManifestGroup(const AssetBankManifest & manifest, const std::string & assetsRoot) override
	{
		// If the manifest has a section for me, read it
		auto sectionIt = manifest.sections.find(manifestTag());
		if(sectionIt == manifest.sections.end())
		{
			// No section for me, I'm empty then
			return true;
		}

		const AssetBankManifest::EntryMap & entries = sectionIt->second;

		for(auto it = entries.cbegin(); it != entries.cend(); ++it)
		{
			// load from (path, name)
			if(!load(it->second, it->first))
			{
				// Failed to load an asset
				return false;
			}
		}

		return true;
	}

	inline std::unordered_map<std::string,T*> begin() const { return m_map.begin(); }
	inline std::unordered_map<std::string,T*> end() const { return m_map.end(); }

private:

	// Internal asset loading routine.
	bool loadAsset(T * asset, const std::string & filePath)
	{
		return loadFromFile(asset, filePath);
	}

	std::unordered_map<std::string,T*> m_map;

};

} // namespace zn

#endif // HEADER_ZN_ASSETMAP_HPP_INCLUDED


