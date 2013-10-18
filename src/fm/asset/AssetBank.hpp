#ifndef HEADER_ZN_ASSETBANK_HPP_INCLUDED
#define HEADER_ZN_ASSETBANK_HPP_INCLUDED
/*
AssetBank.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../util/NonCopyable.hpp"
#include "AssetMap.hpp"
#include "TextureAtlas.hpp"
#include "TiledMap.hpp"

namespace zn
{

// Centralized asset container.
// /!\ Not thread-safe (yet).
class AssetBank : public NonCopyable
{
public:

	AssetMap<sf::Texture>      textures;
	AssetMap<sf::Font>         fonts;
	AssetMap<TextureAtlas>     atlases;
	AssetMap<TiledMap>         maps;
	AssetMap<sf::SoundBuffer>  sounds;

	AssetBank();
	~AssetBank();

	// Sets the root folder of all AssetMaps.
	void setRootFolder(const std::string & rf);

	bool loadFromJSON(const std::string & filePath);
//	bool loadFolder(const std::string & folderPath);

	void makeCurrent();

	static AssetBank * current();

};

} // namespace zn

#endif // HEADER_ZN_ASSETBANK_HPP_INCLUDED


