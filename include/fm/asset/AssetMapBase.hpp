#ifndef HEADER_ZN_ASSETMAPBASE_HPP_INCLUDED
#define HEADER_ZN_ASSETMAPBASE_HPP_INCLUDED

#include <string>
#include <algorithm>
#include <unordered_set>

#include <fm/types.hpp>
#include <fm/json/json_utils.hpp>

namespace zn
{

/// \brief Base class of AssetMaps. Defines non-template common members.
class AssetMapBase
{
public:

	AssetMapBase(std::string pTag) :
		tag(pTag)
	{}

	/// \brief Get how many assets are stored in this map.
	virtual u32 size() const = 0;

	/// \brief Loads a section of typed assets from a JSON manifest data tree.
	/// \param doc: JSON data tree
	/// \param assetsRoot: root folder path of the assets
	/// \see AssetBank.cpp, loadFromJSON().
	virtual bool loadManifestGroup(JsonBox::Value & doc, const std::string & assetsRoot) = 0;

	/// \brief Get the root folder for this map (without the end slash)
	inline const std::string & rootFolder() const { return m_rootFolder; }

	/// \brief Sets the root folder that will be used for loading the assets of the map
	/// \param rf: path to the folder
	void setRootFolder(const std::string & rf);

	/// \brief Loads a list of file extensions to associate from a JSON string array.
	/// Does nothing if the given object is null or not an array.
	void loadFileAssociations(JsonBox::Value & obj);

	/// \brief Name of the type of asset stored in this map.
	/// It can also be used in manifest files.
	const std::string tag;

	/// \brief File extensions associated with the assets this map can store (lower case, without the dot).
	std::unordered_set<std::string> fileExtensions;

protected:

	std::string m_rootFolder;

};

} // namespace zn

#endif // HEADER_ZN_ASSETMAPBASE_HPP_INCLUDED


