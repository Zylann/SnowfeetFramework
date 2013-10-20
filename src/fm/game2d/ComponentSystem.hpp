#ifndef HEADER_ZN_COMPONENTSYSTEM_HPP_INCLUDED
#define HEADER_ZN_COMPONENTSYSTEM_HPP_INCLUDED

#include <iostream>
#include <cassert>
#include "../types.hpp"

namespace zn
{

// Generic container gathering a specific type of component.
// It can be used to interact with a more complex subsystem (physics engine, scene graph library...)
// Note: some systems may need more than one type of component (like the audio system),
// and then will not be based on this generic one.
template <class Cmp_T>
class ComponentSystem
{
public:

	ComponentSystem()
	{}

	virtual ~ComponentSystem()
	{}

	void registerComponent(Cmp_T * cmp)
	{
#ifdef ZN_DEBUG
		if(cmp == nullptr)
			std::cout << "E: ComponentSystem::registerComponent: cannot register null" << std::endl;
#endif
		assert(cmp != nullptr);

#ifdef ZN_DEBUG
		auto it = m_all.find(cmp);
		if(it != m_all.end())
		{
			std::cout << "E: ComponentSystem::registerComponent: already registered ("
					  << cmp->componentType().name << ")" << std::endl;
			return;
		}
#endif
		m_all.insert(cmp);

//		onRegister(cmp);
	}

	void unregisterComponent(Cmp_T * cmp)
	{
		assert(cmp != nullptr);

		if(m_all.erase(cmp) == 0)
		{
			std::cout << "E: Scene::unregisterComponent: "
				<< cmp->componentType().name << " was not registered." << std::endl;
		}
	}

	void update()
	{
		// Iterate over a copy to avoid
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

protected:

	std::unordered_set<Cmp_T*> m_all; // References to all instances of the component in the scene

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTSYSTEM_HPP_INCLUDED



