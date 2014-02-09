#ifndef HEADER_ZN_COMPONENTLIST_HPP_INCLUDED
#define HEADER_ZN_COMPONENTLIST_HPP_INCLUDED

#include <iostream>
#include <cassert>
#include <unordered_set>

#include <fm/types.hpp>

namespace zn
{

// Generic container gathering a specific type of component.
template <class Cmp_T>
class ComponentList
{
public:

	// Adds a component to the list.
	void add(Cmp_T * cmp)
	{
#ifdef ZN_DEBUG
		if(cmp == nullptr)
			std::cout << "E: ComponentList::add: cannot add null" << std::endl;
#endif
		assert(cmp != nullptr);

#ifdef ZN_DEBUG
		auto it = m_all.find(cmp);
		if(it != m_all.end())
		{
			std::cout << "E: ComponentList::add: already contains ("
					  << cmp->componentType().name << ")" << std::endl;
			return;
		}
#endif
		m_all.insert(cmp);
	}

	// Removes an existing component from the list (doesn't deletes it).
	void remove(Cmp_T * cmp)
	{
		assert(cmp != nullptr);

		if(m_all.erase(cmp) == 0)
		{
			std::cout << "E: ComponentList::remove: "
				<< cmp->componentType().name << " was not found." << std::endl;
		}
	}

	// Updates all components in the list if their entity is active.
	// Uses a copy of the internal container to avoid concurrent modifications.
	void update()
	{
		// Iterate over a copy to avoid concurrent modifications
		auto updateList = m_all;

		for(auto it = updateList.begin(); it != updateList.end(); ++it)
		{
			Cmp_T & cmp = **it;
			if(cmp.entity().active())
			{
				cmp.update();
			}
		}
	}

	inline u32 count() const { return m_all.size(); }
	inline bool empty() const { return m_all.empty(); }

	inline typename std::unordered_set<Cmp_T*>::iterator begin() { return m_all.begin(); }
	inline typename std::unordered_set<Cmp_T*>::iterator end() { return m_all.end(); }

	inline typename std::unordered_set<Cmp_T*>::const_iterator cbegin() const { return m_all.cbegin(); }
	inline typename std::unordered_set<Cmp_T*>::const_iterator cend() const { return m_all.cend(); }

private:

	std::unordered_set<Cmp_T*> m_all; // References to all instances of the component in the scene

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTLIST_HPP_INCLUDED



