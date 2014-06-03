#ifndef HEADER_ZN_OBJECTTYPEDATABASE_HPP_INCLUDED
#define HEADER_ZN_OBJECTTYPEDATABASE_HPP_INCLUDED

#include <vector>
#include <unordered_map>
#include <functional>
#include <fm/util/Log.hpp>
#include <fm/reflect/Object.hpp>

namespace zn
{

/// \brief Singleton class that allows the creation of objects from their name or ID.
/// It can be used for serialization and basic reflection.
class ObjectTypeDatabase
{
private:

	// Private to prevent construction from the outside
	ObjectTypeDatabase() :
		m_nextID(1) // IDs start at 1
	{}

	// Prevent copy-construction
	ObjectTypeDatabase(const ObjectTypeDatabase&);

	// Prevent assignment
	ObjectTypeDatabase & operator=(const ObjectTypeDatabase&);

public:

	/// \brief Returns singleton
	static ObjectTypeDatabase & get()
	{
		static ObjectTypeDatabase db; // statically allocated to ensure destruction
		return db;
	}

	/// \brief Registers an object type that uses ZN_OBJECT macro in its definition.
	/// \param userData: optional type information (ObjectType will take the ownership on it!)
	/// \note it generates integer IDs from a counter, then as long as you call
	/// this function in the same order, these IDs will be the same even on different
	/// computers or platforms.
	/// However, IDs may differ if the version of the engine differ too, as there
	/// might be new objects.
	template <class Object_T>
	void registerType(IObjectTypeUserData * userData=nullptr)
	{
		// Get type structure from a static function defined in the object class
		// (that's the case if it uses the ZN_OBJECT macro)
		ObjectType & type = Object_T::sObjectType();

#ifdef ZN_DEBUG
		// Check if the object has already been registered
		if(m_factories.find(type.name) != m_factories.end())
		{
			zn::log.err() << "ObjectTypeDatabase::registerType: "
				"registered the same type twice ! "
				"(" << type.name << ")" << zn::log.endl();
		}
#endif

		type.userData = userData;

		// Generate component type ID
		type.ID = m_nextID++;
		m_nameToID[type.name] = type.ID;

		// Register factory function
		m_factories[type.name] = Object_T::instantiate;

		// Register type
		if(type.ID >= m_registeredTypes.size())
		{
			m_registeredTypes.resize(type.ID+1, nullptr);
		}
		m_registeredTypes[type.ID] = &type;

#ifdef ZN_DEBUG
		zn::log.debug() << "Registered " << type.toString() << zn::log.endl();
#endif
	}

	/// \brief Creates a new instance of an object from its name.
	/// It does the same thing as "new MyObject()", where className = "MyObject".
	/// \return pointer to dynamically allocated object instance, or null if the object
	/// couldn't be allocated (as for abstract types).
	Object * instantiate(const std::string className);

	/// \brief Gets an object metaclass from its name
	/// \return the object type, or null if not found.
	ObjectType * getType(const std::string & typeName);

private:

	std::vector<ObjectType*> m_registeredTypes;
	std::unordered_map<std::string,ObjectTypeID> m_nameToID;
	std::unordered_map<std::string, std::function<Object*()>> m_factories;
	u32 m_nextID;

};

} // namespace zn

#endif // HEADER_ZN_OBJECTTYPEDATABASE_HPP_INCLUDED

