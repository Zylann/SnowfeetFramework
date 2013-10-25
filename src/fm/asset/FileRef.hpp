#ifndef HEADER_ZN_FILEREF_HPP_INCLUDED
#define HEADER_ZN_FILEREF_HPP_INCLUDED

#include <string>
#include <fstream>
#include "Asset.hpp"

namespace zn
{

// Sometimes, a file may not be fully loaded into memory, but just referenced
// into the asset bank (as for media streams).
struct FileRef : public IAsset
{
	bool loadFromFile(const std::string & pFilePath) override
	{
		filePath = pFilePath;
		std::ifstream ifs(pFilePath);
		return ifs.good(); // Just returns if the file exists
	}

	std::string filePath;

};

} // namespace zn

#endif // HEADER_ZN_FILEREF_HPP_INCLUDED


