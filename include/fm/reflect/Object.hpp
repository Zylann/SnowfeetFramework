#ifndef HEADER_ZN_OBJECT_HPP_INCLUDED
#define HEADER_ZN_OBJECT_HPP_INCLUDED

#include <fm/reflect/ObjectType.hpp>

// Constraints of the reflection system:
// Classes must define a parameter-less constructor
// Single inheritance only
// Type IDs must be the same on whatever platform or compiler
// Must be usable from application scripts (not only native code)

#define _ZN_OBJECT(__name, __baseName)                                         \
	static zn::ObjectType & sObjectType() {                                    \
		static zn::ObjectType t(#__name, #__baseName);                         \
		return t;                                                              \
	}                                                                          \
	const zn::ObjectType & objectType() const override {                       \
		return sObjectType();                                                  \
	}                                                                          \

// Put this declaration in public in a class inheriting from Object.
// Note: only single inheritance is supported.
// __name: class name with full namespace (must be unique !)
// __baseName: inherited class name with full namespace
#define ZN_OBJECT(__name, __baseName)                                          \
	_ZN_OBJECT(__name, __baseName)                                              \
	static zn::Object * instantiate() {                                        \
		return new __name();                                                   \
	}

// Variant for abstract types
#define ZN_ABSTRACT_OBJECT(__name, __baseName)                                 \
	_ZN_OBJECT(__name, __baseName)                                              \
	static zn::Object * instantiate() {                                        \
		return nullptr;                                                        \
	}

namespace zn
{

class Object
{
public:

	//ZN_ABSTRACT_OBJECT(zn::Object,)

	/// \brief base metaclass.
	static zn::ObjectType & sObjectType()
	{
		static zn::ObjectType t("zn::Object", "");
		return t;
	}

	/// \brief get the metaclass of the object.
	virtual const zn::ObjectType & objectType() const
	{
		return sObjectType();
	}

	/// \brief Tests if the object is an instance of the given type.
	/// It also works for inherited types, if they share the same reflection system.
	template <class Object_T>
	bool isInstanceOf()
	{
		ObjectType & expectedType = Object_T::sObjectType();
		return objectType().is(expectedType);
	}
};

} // namespace zn

#endif // HEADER_ZN_OBJECT_HPP_INCLUDED




