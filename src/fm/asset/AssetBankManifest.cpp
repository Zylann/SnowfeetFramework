#include <fm/asset/AssetBankManifest.hpp>
#include <fm/json/json_utils.hpp>
#include <fm/util/Log.hpp>
#include <fm/util/stringutils.hpp>

using namespace std;

namespace zn
{

//------------------------------------------------------------------------------
bool AssetBankManifest::loadFromJSONFile(const std::string & filePath)
{
	// Read JSON file
	JsonBox::Value jsonData;
	if(!loadFromFile(jsonData, filePath, -1, false))
	{
		return false;
	}

	// Collect data
	const JsonBox::Object & jsonObj = jsonData.getObject();
	for(auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
	{
		if(it->second.isArray())
		{
			JsonBox::Value & jsonArray = jsonData[it->first];

			EntryMap & entries = sections[it->first];

			for(u32 i = 0; i < jsonArray.getArray().size(); ++i)
			{
				if(jsonArray[i].isObject())
				{
					JsonBox::Value & e = jsonArray[i];

					std::string src = e["src"].getString();
					std::string name = e["name"].getString();

					if(src.empty())
					{
						// src musn't be empty. Skip.
						log.err() << "AssetBankManifest: found an empty src parameter, skipping asset." << log.endl();
						continue;
					}

					// If the asset name is empty
					if(name.empty())
					{
						// Use the file name without extension
						name = fileNameWithoutExtension(src);
					}

					entries.insert(std::make_pair(name, src));
				}
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------------
bool AssetBankManifest::saveToJSONFile(const string & filePath) const
{
	// Note: the JsonBox writer is not used here because it creates too much white newlines.

	// Open or create file
	ofstream ofs(filePath, ios::out|ios::trunc|ios::binary);
	if(!ofs.good())
	{
		log.err() << "Couldn't write to file \"" << filePath << '"' << log.endl();
		return false;
	}

	ofs << "{" << endl;

	// Write sections
	u32 j = 0;
	for(auto sectionIt = sections.cbegin(); sectionIt != sections.cend(); ++sectionIt)
	{
		ofs << "\t\"" << sectionIt->first << "\":[" << endl;

		const EntryMap & section = sectionIt->second;

		// Write entries of the section
		JsonBox::Array jsonArray;
		u32 i = 0;
		for(auto entryIt = section.cbegin(); entryIt != section.cend(); ++entryIt)
		{
			ofs << "\t\t{ ";

			ofs << "\"src\":\"" << entryIt->second << "\"";

			if(!entryIt->first.empty())
			{
				ofs << ", \"name\":\"" << entryIt->first << "\"";
			}

			if(i == section.size()-1)
			{
				ofs << " }" << endl;
			}
			else
			{
				ofs << " }," << endl;
			}

			++i;
		}

		if(j == sections.size()-1)
		{
			ofs << "\t]" << endl;
		}
		else
		{
			ofs << "\t]," << endl;
		}

		++j;
	}

	ofs << "}" << endl;

	ofs.close();

	return true;
}

//------------------------------------------------------------------------------
void AssetBankManifest::merge(const AssetBankManifest & other, bool overwrite)
{
	// For each section in the other manifest
	for(auto otherSectionIt = other.sections.cbegin();
		otherSectionIt != other.sections.cend();
		++otherSectionIt)
	{
		const EntryMap & otherSection = otherSectionIt->second;

		// Find if the manifest has the section
		auto sectionIt = sections.find(otherSectionIt->first);

		// If the manifest doesn't have this section
		if(sectionIt == sections.end())
		{
			// Add section
			sections[otherSectionIt->first] = otherSection;
		}
		else
		{
			EntryMap & section = sectionIt->second;

			// Merge sections
			for(auto otherEntryIt = otherSection.cbegin();
				otherEntryIt != otherSection.cend();
				++otherEntryIt)
			{
				auto entryIt = section.find(otherEntryIt->first);

				// If the entry doesn't exist
				if(entryIt == section.end())
				{
					// Add it
					section[otherEntryIt->first] = otherEntryIt->second;
				}
				else
				{
					if(overwrite)
					{
						// Overwrite previous one
						entryIt->second = otherEntryIt->second;
					}
				}
			}
		}
	}
}

} // namespace zn




