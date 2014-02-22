#include <fm/asset/AssetMapBase.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void AssetMapBase::setRootFolder(const std::string & rf)
{
#ifdef ZN_DEBUG
	if(size() != 0)
	{
		std::cout << "W: AssetMap: root folder changed while assets are already loaded." << std::endl;
	}
#endif
	m_rootFolder = rf;
	// Remove the '/' if it is present
	if(!m_rootFolder.empty() && m_rootFolder[m_rootFolder.size()-1] == '/')
	{
		m_rootFolder = m_rootFolder.substr(0, m_rootFolder.size()-1);
	}
}

//------------------------------------------------------------------------------
void AssetMapBase::loadFileAssociations(JsonBox::Value & obj)
{
	if(!obj.isArray())
	{
		// Do nothing if the object is not an array (or is null)
		return;
	}

	u32 arraySize = obj.getArray().size();
	for(u32 i = 0; i < arraySize; ++i)
	{
		std::string ext = obj[i].getString();
		if(!ext.empty())
		{
			// Add lower case extension
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			fileExtensions.insert(ext);
		}
#ifdef ZN_DEBUG
		else
		{
			std::cout << "W: AssetMap::loadFileAssociations: "
				"[" << tag << "] found empty string" << std::endl;
		}
#endif
	}
}

} // namespace zn

