/*
AssetBank.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the Plane framework project.
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
#include <fm/asset/AssetBankManifest.hpp>

namespace zn
{

/// \brief Centralized asset database.
/// \note /!\ Not thread-safe (yet).
class ZN_API AssetBank : public NonCopyable
{
public:

	// TODO complete doc

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

	/// \brief Open or creates and indexes asset manifests, then loads assets contained in a folder.
	/// \param rootFolder: root folder of the assets
	/// \param performAutoIndex: recursive search for files to add to the manifest, based on file associations
	/// \return true if success, false otherwise
	bool load(const std::string & rootFolder, bool performAutoIndex=false);

	// Sets the root folder containing all the assets.
	// This folder will be prepended to every assets to be loaded.
	void setRootFolder(const std::string & rootFolder);

	bool autoIndex(const std::string & folderPath, AssetBankManifest & manifest);

	bool loadAssets(AssetBankManifest & manifest);

	const AssetMapBase * findAssetType(const std::string & filePath);

	/// \brief Loads file-extension <--> asset-type associations from a JSON file.
	/// \note Using a manifest file or meta-files doesn't needs to know file associations.
	/// This function is useful if you want the engine to auto-detect your assets.
	/// \param assocFile: path to the JSON file containing the associations.
	/// \param create: If the file doesn't exist, create it
	/// \return true if success, false if error.
	bool loadFileAssociations(const std::string & assocFile, bool create);

	// Sets this AssetBank as the current one for global access
	void makeCurrent();

	// Gets the active AssetBank
	static AssetBank * current();

private:

	/// \brief Creates a default file-associations JSON object.
	/// \param doc: empty JSON data.
	void createDefaultFileAssociations(JsonBox::Value & doc);

	/// \brief Files matching this will be ignored first.
	AssetFileMatcher m_ignoredFilesMatcher;

	std::string m_root;

	/// \brief References all asset containers under the same map type for bulk operations
	std::vector<AssetMapBase*> m_assetMaps;

	/// \brief Name of the manifest file (including extension).
	const std::string m_manifestFileName;

	/// \brief Name of the file associations file (including extension).
	const std::string m_assocFileName;

};

} // namespace zn

#endif // HEADER_ZN_ASSETBANK_HPP_INCLUDED


