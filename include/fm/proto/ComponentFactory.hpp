#ifndef HEADER_ZN_COMPONENTFACTORY_HPP_INCLUDED
#define HEADER_ZN_COMPONENTFACTORY_HPP_INCLUDED

#include <unordered_map>
#include <fm/proto/ComponentType.hpp>

namespace zn
{

// Singleton class that allows the creation of components from their name or ID
class ComponentFactory
{
private:

	// Private to prevent construction from the outside
	ComponentFactory() {}

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

	// Functions below allow you to register components in the factory system,
	// which can then be created from their name as strings.
	// This is mainly used for serialization.

	// Registers a component type by a name and a function that instantiates this type.
	// You must always use the full name of the class in the C++ way of things.
	void registerType(const std::string className, std::function<AComponent*()> factory);

	// Creates a new instance of a component from its name.
	// It does the same thing as "new MyComponent()", where className = "MyComponent".
	AComponent * instantiate(const std::string className);

	// Registers built-in components. Must be called once.
	static void registerEngineComponents();

private:

	std::unordered_map<std::string, std::function<AComponent*()>> m_factories;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTFACTORY_HPP_INCLUDED


