/*
AssetMap.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
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

	/// \brief constructs an empty AssetMap with a tag.
	/// \param pTag: tag identifying the type of asset stored.
	/// It must be unique and not empty. It will be used to identify the type of
	/// asset in places such as data files.
	AssetMap(std::string pTag) : AssetMapBase(pTag)
	{}

	/// \brief Destroys the AssetMap and frees the assets it contains.
	~AssetMap()
	{
		clear();
	};

	/// \brief Get the number of assets stored in this AssetMap.
	/// \return asset count.
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

	/// \brief Gets an asset from its name.
	/// \param name: Unique name of the asset within the container.
	/// \return Pointer to the asset, or nullptr if it hasn't been found.
	T * get(const std::string & name) const
	{
		auto it = m_map.find(name);
		if(it != m_map.end())
		{
			return it->second;
		}
		else
		{
#ifdef ZN_DEBUG
			log.err() << "AssetMap::get: not found \"" << name << '"' << log.endl();
#endif
			return nullptr;
		}
	}

	/// \brief Finds the name of an asset from its pointer.
	/// \param asset: pointer to the asset.
	/// \return The unique name of the asset within the container,
	/// or an empty string if the asset is not contained in the map.
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

	/// \brief Loads an asset from a file into the map and gives it a unique name.
	/// Does nothing if the name is already in use.
	/// \param filePath: path to the asset, relative to the assets root folder.
	/// \param name: Unique name to give to the asset.
	/// \return true if the asset has been loaded or if the name is in use,
	/// false if the name is empty or if the asset couldn't be loaded.
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
		{
			log.warn() << "AssetMap::load: \"" << name << "\" has been loaded twice, skipping." << log.endl();
			return true; // Already loaded
		}

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

	bool loadFromManifest(const AssetBankManifest & manifest, const std::string & assetsRoot) override
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

	/// \brief begin iterator to allow iteration over the assets.
	inline std::unordered_map<std::string,T*> begin() const { return m_map.begin(); }

	/// \brief end iterator to allow iteration over the assets.
	inline std::unordered_map<std::string,T*> end() const { return m_map.end(); }

private:

	/// \brief Internal asset loading routine.
	bool loadAsset(T * asset, const std::string & filePath)
	{
		return loadFromFile(asset, filePath);
	}

	/// \brief All the assets stored by name.
	std::unordered_map<std::string,T*> m_map;

};

} // namespace zn

#endif // HEADER_ZN_ASSETMAP_HPP_INCLUDED


