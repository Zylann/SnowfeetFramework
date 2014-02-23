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

	/// \brief Contains all loaded textures.
	AssetMap<sf::Texture> textures;

	/// \brief Contains all loaded shaders.
	AssetMap<sf::Shader> shaders;

	/// \brief Contains all loaded fonts.
	AssetMap<sf::Font> fonts;

	/// \brief Contains all loaded atlases.
	AssetMap<TextureAtlas> atlases;

	/// \brief Contains all loaded maps.
	AssetMap<TiledMap> maps;

	/// \brief Contains all loaded sounds.
	AssetMap<sf::SoundBuffer> soundBuffers; // TODO AssetBank: merge sound types into one

	/// \brief Contains all streamed sounds as file references.
	AssetMap<FileRef> soundStreams;

	/// \brief Contains all loaded materials.
	AssetMap<Material> materials;

	/// \brief Constructs an empty AssetBank.
	AssetBank();

	/// \brief Destroys the AssetBank and frees all the assets it contains.
	~AssetBank();

	/// \brief Given a root folder, loads every asset contained in it by reading a special manifest file.
	/// if the manifest is not present, the folder can be scanned for asset files and the manifest will be generated.
	/// Asset file recognition works with pattern matching applided on file paths.
	/// Patterns are loaded from a second special file. If it is not found, a default one will be created.
	/// \param rootFolder: root folder of the assets
	/// \param rescan: enables folder scanning if no manifest is found, or if you want to update the existing one.
	/// \return true if success, false if an error occurred.
	bool load(const std::string & rootFolder, bool rescan=false);

	/// \brief Sets the root folder containing all the assets.
	/// This folder will be prepended to every assets to be loaded.
	/// \note this function is called by load().
	void setRootFolder(const std::string & rootFolder);

	/// \brief Scans the root folder and its subdirectories for asset files,
	/// and puts them in a manifest.
	/// \param folderPath: the root folder to scan, usually the one from this AssetBank.
	/// \param manifest: manifest file to fill
	/// \return true if succeeded, false if an error occurred
	bool scanFolder(const std::string & folderPath, AssetBankManifest & manifest);

	/// \brief Effectively loads all assets described in the given manifest.
	/// \param manifest: list of files to load
	/// \return true if every asset has been loaded, false if one failed.
	bool loadAssets(AssetBankManifest & manifest);

	/// \brief Finds the asset type associated to the given file.
	/// \param filePath: path to the file
	/// \return AssetMap associated with the file
	const AssetMapBase * findAssetType(const std::string & filePath);

	/// \brief Loads file-extension <--> asset-type associations from a JSON file.
	/// \note Using a manifest file or meta-files doesn't needs to know file associations.
	/// This function is useful if you want the engine to auto-detect your assets.
	/// \param assocFile: path to the JSON file containing the associations.
	/// \param create: If the file doesn't exist, create it
	/// \return true if success, false if error.
	bool loadFileAssociations(const std::string & assocFile, bool create);

	/// \brief Sets this AssetBank as the current one for global access
	void makeCurrent();

	/// \brief Gets the active AssetBank
	static AssetBank * current();

private:

	/// \brief Creates a default file-associations JSON object.
	/// \param doc: empty JSON data.
	void createDefaultFileAssociations(JsonBox::Value & doc);

	/// \brief Files matching this will be ignored first.
	AssetFileMatcher m_ignoredFilesMatcher;

	/// \brief root folder of the AssetBank, relative to the execution's folder.
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


