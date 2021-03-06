#include <fstream>
#include <fm/json/json_utils.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

bool loadFromFile(JsonBox::Value & document, const std::string & filePath, s32 checkVersion, bool openError)
{
	std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
	if(!ifs.good())
	{
		if(openError)
		{
			log.err() << "loadFromFile: "
				"couldn't open JSON file \"" + filePath + '"' << log.endl();
		}
		return false;
	}

#ifdef ZN_DEBUG
	std::cout << "D: Parsing \"" << filePath << '"' << std::endl;
#endif

	// Parse stream

	document.loadFromStream(ifs);
	ifs.close();

	// Check document

	assert(document.isObject());

	if(checkVersion >= 0)
	{
		s32 version = document["version"].getInt();
		if(version != checkVersion)
		{
			log.err() << "loadFromFile: JSON not compatible "
				"(version=" << version << ", expected " << version << ") "
				"into file \"" << filePath << ". "
				"Maybe this file is from another version of the program?"
				<< log.endl();
			return false;
		}
	}

	return true;
}

} // namespace zn


