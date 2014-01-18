/*
AssetBank.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_ASSETBANK_HPP_INCLUDED
#define HEADER_ZN_ASSETBANK_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fm/util/NonCopyable.hpp>
#include <fm/asset/AssetMap.hpp>
#include <fm/asset/TextureAtlas.hpp>
#include <fm/asset/TiledMap.hpp>
#include <fm/asset/FileRef.hpp>

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
	AssetMap<sf::SoundBuffer>  soundBuffers; // TODO AssetBank: merge sound types into one
	AssetMap<FileRef>          soundStreams;

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


