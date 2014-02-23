#ifndef HEADER_ZN_ASSETBANKMANIFEST_HPP_INCLUDED
#define HEADER_ZN_ASSETBANKMANIFEST_HPP_INCLUDED

#include <unordered_map>
#include <string>
#include <vector>

#include <fm/config.hpp>

namespace zn
{

/// \brief represents the index of all assets files of a folder.
/// Each file is represented as a path and a name (or alias). Both are mandatory.
/// If the name of a file is not specified, it is deduced from its path.
class ZN_API AssetBankManifest
{
public:

	/// \brief Entries by path. An entry is a reference to a file.
	/// (name) -> (path to the file)
	typedef std::unordered_map<std::string, std::string> EntryMap;

	/// \brief Sections by name. A section is a set of entries.
	/// (asset type) -> (list of its entries)
	typedef std::unordered_map<std::string, EntryMap> SectionMap;

	/// \brief Loads the manifest from a JSON file
	/// \param filePath: path to the file
	/// \return True if success, false if error
	bool loadFromJSONFile(const std::string & filePath);

	/// \brief Saves the manifest to a JSON file. If it exists already, it is overwritten.
	/// \param filePath: path to the file
	/// \return True if success, false if error.
	bool saveToJSONFile(const std::string & filePath) const;

	/// \brief Adds contents from another manifest to this one.
	/// \param other: other manifest to take the contents from.
	/// \param overwrite: sets wether existing entries in the manifest should be overwritten or not.
	void merge(const AssetBankManifest & other, bool overwrite=true);

	SectionMap sections;

};

} // namespace zn

#endif // HEADER_ZN_ASSETBANKMANIFEST_HPP_INCLUDED


