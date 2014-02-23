#include <fm/asset/AssetFileMatcher.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void AssetFileMatcher::addPattern(std::string simplePattern)
{
	std::string regexPattern;

	for(auto it = simplePattern.begin(); it != simplePattern.end(); ++it)
	{
		char c = *it;

		switch(c)
		{
		// '*' means one or more characters
		case '*': regexPattern += ".+"; break;
		// '.' is part of file extensions, escape it
		case '.': regexPattern += "\\."; break;

		default: regexPattern += c; break;
		}
	}

	try
	{
		std::regex reg(regexPattern, std::regex_constants::ECMAScript);
		m_patterns.push_back(std::regex(reg));
	}
	catch(std::exception & e)
	{
		log.err() << "AssetFileMatcher: " << e.what() << " on \"" << regexPattern << "\"" << log.endl();
	}
}

//------------------------------------------------------------------------------
bool AssetFileMatcher::evaluate(const std::string & str)
{
	for(auto it = m_patterns.begin(); it != m_patterns.end(); ++it)
	{
		const std::regex & pattern = *it;

		if(std::regex_match(str, pattern))
		{
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------
void AssetFileMatcher::loadFromJSON(JsonBox::Value obj)
{
	if(!obj.isArray())
	{
		// Do nothing if the object is not an array (or is null)
		return;
	}

	u32 arraySize = obj.getArray().size();
	for(u32 i = 0; i < arraySize; ++i)
	{
		std::string expression = obj[i].getString();
		if(!expression.empty())
		{
			addPattern(expression);
		}
#ifdef ZN_DEBUG
		else
		{
			log.warn() << "AssetFileMatcher::loadFromJSON: "
				"found empty string" << log.endl();
		}
#endif
	}
}

} // namespace zn


