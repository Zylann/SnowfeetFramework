#ifndef HEADER_ZN_ASSETMAPBASE_HPP_INCLUDED
#define HEADER_ZN_ASSETMAPBASE_HPP_INCLUDED

#include <string>
#include <algorithm>
#include <unordered_set>

#include <fm/types.hpp>
#include <fm/json/json_utils.hpp>
#include <fm/asset/AssetBankManifest.hpp>
#include <fm/asset/AssetFileMatcher.hpp>

namespace zn
{

/// \brief Base class of AssetMaps. Defines non-template common members.
class AssetMapBase
{
public:

	AssetMapBase(const std::string & pTag);

	/// \brief Get how many assets are stored in this map.
	virtual u32 size() const = 0;

	/// \brief Loads a section of typed assets from a JSON manifest data tree.
	/// \param doc: JSON data tree
	/// \param assetsRoot: root folder path of the assets
	/// \see AssetBank.cpp, loadFromJSON().
	virtual bool loadManifestGroup(const AssetBankManifest & manifest, const std::string & assetsRoot) = 0;

	/// \brief Get the root folder for this map (without the end slash)
	inline const std::string & rootFolder() const { return m_rootFolder; }

	/// \brief Sets the root folder that will be used for loading the assets of the map
	/// \param rf: path to the folder
	void setRootFolder(const std::string & rf);

	/// \brief Loads a list of expressions used to match files associated with this AssetMap.
	/// Does nothing if the given object is null or not an array.
	/// \see AssetFileMatcher.hpp
	void loadFileAssociations(JsonBox::Value & obj);

	/// \brief Gets the tag used to name the list of assets in a manifest file
	inline const std::string & manifestTag() const { return m_manifestTag; }

	/// \brief Name of the type of asset stored in this map.
	/// It can also be used in manifest files.
	const std::string tag;

	/// \brief Defines how to recognize asset files associated with this AssetMap.
	AssetFileMatcher matcher;

protected:

	std::string m_rootFolder;

	/// \brief Tag used to name the list of assets in a manifest file
	std::string m_manifestTag;

};

} // namespace zn

#endif // HEADER_ZN_ASSETMAPBASE_HPP_INCLUDED


