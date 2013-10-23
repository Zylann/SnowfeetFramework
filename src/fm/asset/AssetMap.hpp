/*
AssetMap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

/*
AssetMap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_ASSETMAP_HPP_INCLUDED
#define HEADER_ZN_ASSETMAP_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <iostream>

#include "../config.hpp"
#include "../rapidjson/document.h"
#include "../rapidjson/filestream.h"

namespace zn
{

// This generic class handles the loading of specific-type assets and indexes them in a map.
// assets with the same file path are not reloaded.
// The template parameter must be an asset class implementing functions defined in IAsset
// (such as bool loadFromFile(const std::string & filePath) )
template <class T>
class AssetMap
{
public:

	~AssetMap()
	{
		clear();
	};

	void clear()
	{
		for(auto it = m_map.begin(); it != m_map.end(); ++it)
			delete it->second;
		m_map.clear();
	}

	// Get the root folder (without the end slash)
	inline const std::string & rootFolder() const { return m_rootFolder; }

	// Sets the root folder that will be used for loading the assets of this type
	void setRootFolder(const std::string & rf)
	{
#ifdef ZN_DEBUG
		if(!m_map.empty())
			std::cout << "D: AssetMap: root folder changed while assets are already loaded." << std::endl;
#endif
		m_rootFolder = rf;
		// Remove the '/' if it is present
		if(!m_rootFolder.empty() && m_rootFolder[m_rootFolder.size()-1] == '/')
		{
			m_rootFolder = m_rootFolder.substr(0, m_rootFolder.size()-1);
		}
	}

	T * get(const std::string & id) const
	{
		auto it = m_map.find(id);
		if(it != m_map.end())
			return it->second;
		else
		{
#ifdef ZN_DEBUG
			std::cout << "D: AssetMap::get: not found " << id << std::endl;
#endif
			return nullptr;
		}
	}

	bool load(std::string filePath, const std::string & id)
	{
		if(id.empty())
		{
#ifdef ZN_DEBUG
			std::cout << "E: AssetMap::load: empty ID specified !" << std::endl;
#endif
			return false;
		}

		if(m_map.find(id) != m_map.end())
			return true; // Already loaded

		if(!m_rootFolder.empty())
			filePath = m_rootFolder + '/' + filePath;
#ifdef ZN_DEBUG
		std::cout << "D: AssetMap: loading " << filePath << std::endl;
#endif

		T * asset = new T();
		if(!loadAsset(asset, filePath))
		{
			delete asset;
			return false;
		}
		else
		{
			m_map[id] = asset;
			return true;
		}
	}

	bool loadList(const rapidjson::Value & obj)
	{
		if(obj.HasMember("root"))
		{
			setRootFolder(obj["root"].GetString());
		}

		const rapidjson::Value & jlist = obj["list"];

		std::string name, src;

		for(rapidjson::SizeType i = 0; i < jlist.Size(); ++i)
		{
			// TODO AssetMap: if name is not specified, use the filename without extension
			name = jlist[i]["name"].GetString();
			src = jlist[i]["src"].GetString();

			if(!load(src, name))
			{
				return false;
			}
		}

		return true;
	}

	inline std::unordered_map<std::string,T*> begin() const { return m_map.begin(); }
	inline std::unordered_map<std::string,T*> end() const { return m_map.end(); }

private:

	// Internal asset loading routine.
	// If T has a different loading method, a template specialization may be required.
	inline bool loadAsset(T * asset, const std::string & filePath)
	{
		return asset->loadFromFile(filePath);
	}

	std::string m_rootFolder;
	std::unordered_map<std::string,T*> m_map;
};

#include <SFML/Audio.hpp>

// Specialization for sf::Music
template <>
inline bool AssetMap<sf::Music>::loadAsset(sf::Music * asset, const std::string & filePath)
{
	// Music is not really loaded, it is streamed
	return asset->openFromFile(filePath);
}

} // namespace zn

#endif // HEADER_ZN_ASSETMAP_HPP_INCLUDED


