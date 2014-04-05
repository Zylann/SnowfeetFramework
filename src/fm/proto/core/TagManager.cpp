#include <fm/proto/core/TagManager.hpp>
#include <fm/proto/core/Entity.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

//------------------------------------------------------------------------------
TagManager & TagManager::setTag(u8 index, const std::string & name)
{
#ifdef ZN_DEBUG
	if(index >= MAX_TAGS)
	{
		log.err() << "TagManager::setTag: "
			"given index (" << (u32)index << ") is out of bounds!" << log.endl();
	}
#endif
	assert(index < MAX_TAGS);

	auto it = m_name2Tag.find(name);
#ifdef ZN_DEBUG
	if(it != m_name2Tag.end())
	{
		log.err() << "TagManager::setTag: "
			"two tags cannot have the same name! \"" << name << "\"" << log.endl();
	}
#endif
	assert(it == m_name2Tag.end());

	m_tags[index] = name;

	return *this;
}

//------------------------------------------------------------------------------
const std::string & TagManager::nameFromIndex(u8 index) const
{
#ifdef ZN_DEBUG
	if(index >= MAX_TAGS)
	{
		log.err() << "TagManager::nameFromIndex: "
			"given index (" << (u32)index << ") is out of bounds!" << log.endl();
	}
#endif
	assert(index < MAX_TAGS);
	return m_tags[index];
}

//------------------------------------------------------------------------------
s8 TagManager::indexFromName(const std::string& name) const
{
	auto it = m_name2Tag.find(name);
	if(it == m_name2Tag.end())
	{
		log.err() << "TagManager::indexFromName: "
			"undefined tag name \"" << name << "\"" << log.endl();
		return -1;
	}
	return it->second;
}

//------------------------------------------------------------------------------
const std::unordered_set<Entity*> & TagManager::taggedEntities(u8 tagIndex) const
{
#ifdef ZN_DEBUG
	if(tagIndex >= MAX_TAGS)
	{
		log.err() << "TagManager::getTaggedEntities: "
			"given index (" << (u32)tagIndex << ") is out of bounds!" << log.endl();
	}
#endif
	assert(tagIndex < MAX_TAGS);
	return m_taggedEntities[tagIndex];
}

//------------------------------------------------------------------------------
void TagManager::onEntityTagged(Entity* e, u8 tagIndex)
{
	m_taggedEntities[tagIndex].insert(e);
}

//------------------------------------------------------------------------------
void TagManager::onEntityUntagged(Entity* e, u8 tagIndex)
{
	m_taggedEntities[tagIndex].erase(e);
}

//------------------------------------------------------------------------------
void TagManager::print(std::ostream& os) const
{
	os << "---------- TagManager ----------" << std::endl;
	for(u32 i = 0; i < MAX_TAGS; ++i)
	{
		os << "[" << i << "] \"" << m_tags[i] << "\", "
			<< m_taggedEntities[i].size() << " entities" << std::endl;
	}
	os << "----------" << std::endl;
}

//------------------------------------------------------------------------------
void TagManager::serialize(JsonBox::Value & o)
{
	JsonBox::Array a;
	a.resize(MAX_TAGS);
	for(u32 i = 0; i < MAX_TAGS; ++i)
	{
		a[i] = m_tags[i];
	}
	o = a;
}

//------------------------------------------------------------------------------
void TagManager::unserialize(JsonBox::Value & o)
{
	const u32 n = o.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		setTag(i, o[i].getString());
	}
}

} // namespace zn



