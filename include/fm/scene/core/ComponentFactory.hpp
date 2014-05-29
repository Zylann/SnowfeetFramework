#ifndef HEADER_ZN_COMPONENTFACTORY_HPP_INCLUDED
#define HEADER_ZN_COMPONENTFACTORY_HPP_INCLUDED

#include <unordered_map>
#include <fm/scene/core/ComponentType.hpp>

namespace zn
{

// Singleton class that allows the creation of components from their name or ID.
// It is used for serialization and component meta-identification.
class ComponentFactory
{
private:

	// Private to prevent construction from the outside
	ComponentFactory() :
		m_nextID(1) // IDs start at 1
	{}

	// Prevent copy-construction
	ComponentFactory(const ComponentFactory&);

	// Prevent assignment
	ComponentFactory & operator=(const ComponentFactory&);

public:

	// Returns ComponentFactory singleton
	static ComponentFactory & get()
	{
		static ComponentFactory factory; // statically allocated to ensure destruction
		return factory;
	}

	// Registers a component type using ZN_COMPONENT macro in its definition.
	// Important: it generates integer IDs from a counter, then as long as you call
	// this function in the same order, these IDs will be the same even on different
	// computers or platforms.
	// However, IDs may differ if the version of the engine differ too, as there
	// might be new components.
	template <class Component_T>
	void registerType()
	{
		// Get type structure from a static function defined in the component class
		// (that's the case if it uses the ZN_COMPONENT macro)
		ComponentType & cmpType = Component_T::sComponentType();

#ifdef ZN_DEBUG
		// Check if the component has already been registered
		if(m_factories.find(cmpType.name) != m_factories.end())
		{
			std::cout << "E: ComponentFactory::registerType: "
				"registered the same component type twice ! "
				"(" << cmpType.name << ")" << std::endl;
		}
#endif

		// Generate component type ID
		cmpType.ID = m_nextID++;

		// Register factory function
		m_factories[cmpType.name] = Component_T::instantiate;

#ifdef ZN_DEBUG
		std::cout << "D: Registered ";
		cmpType.print(std::cout);
		std::cout << std::endl;
#endif
	}

	// Creates a new instance of a component from its name.
	// It does the same thing as "new MyComponent()", where className = "MyComponent".
	AComponent * instantiate(const std::string className);

	// Registers built-in components. Must be called once.
	static void registerEngineComponents();

private:

	std::unordered_map<std::string, std::function<AComponent*()>> m_factories;
	u32 m_nextID;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTFACTORY_HPP_INCLUDED


