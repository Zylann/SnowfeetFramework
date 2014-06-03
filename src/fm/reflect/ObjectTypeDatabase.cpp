#include <cassert>
#include <fm/reflect/ObjectTypeDatabase.hpp>

namespace zn
{

//------------------------------------------------------------------------------
Object * ObjectTypeDatabase::instantiate(const std::string className)
{
	auto it = m_factories.find(className);
#ifdef ZN_DEBUG
	if(it == m_factories.end())
	{
		zn::log.err() << "cannot instantiate " << className << ", factory not found" << zn::log.endl();
	}
#endif
	assert(it != m_factories.end());
	return it->second();
}

//------------------------------------------------------------------------------
ObjectType * ObjectTypeDatabase::getType(const std::string & typeName)
{
	auto it = m_nameToID.find(typeName);
	if(it != m_nameToID.end())
	{
		return m_registeredTypes[it->second];
	}
	else
	{
		return nullptr;
	}
}

} // namespace zn

