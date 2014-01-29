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
#include <fm/asset/Material.hpp>

namespace zn
{

// Centralized asset container.
// /!\ Not thread-safe (yet).
class ZN_API AssetBank : public NonCopyable
{
public:

	AssetMap<sf::Texture>      textures;
	AssetMap<sf::Shader>       shaders;
	AssetMap<sf::Font>         fonts;
	AssetMap<TextureAtlas>     atlases;
	AssetMap<TiledMap>         maps;
	AssetMap<sf::SoundBuffer>  soundBuffers; // TODO AssetBank: merge sound types into one
	AssetMap<FileRef>          soundStreams;
	AssetMap<Material>         materials;

	AssetBank();
	~AssetBank();

	// Sets the root folder containing all the assets.
	// This folder will be prepended to every assets to be loaded.
	void setRootFolder(const std::string & rootFolder);

	// Loads all the assets contained in a manifest JSON file.
	// You must specify a path relative to the executable.
	bool loadFromJSON(const std::string & manifestPath);

//	bool loadFolder(const std::string & folderPath);

	// Sets this AssetBank as the current one for global access
	void makeCurrent();

	// Gets the active AssetBank
	static AssetBank * current();

private:

	std::string m_root;

};

} // namespace zn

#endif // HEADER_ZN_ASSETBANK_HPP_INCLUDED


