/*
ComponentType.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED
#define HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED

#include "../types.hpp"
#include <string>
#include <iostream>
#include <functional>

// Shortcut macros

// TODO Automatic component registration by instancing a static object + using a singleton

// This macro is useful if you are extending a engine kind of component.
// Place this in public in all your complete-type components class declarations.
// Components defined this way are the only ones of their group in an entity.
// __name: class name with full namespace
// __group: group from enum ComponentGroup
// __updateOrder: update order of the component
#define ZN_COMPONENT(__name, __group, __updateOrder)                           \
	static zn::ComponentType & sComponentType()                                \
	{                                                                          \
		static zn::ComponentType cmpt(                                         \
			#__name, __group, __updateOrder                                    \
		);                                                                     \
		return cmpt;                                                           \
	}                                                                          \
	const zn::ComponentType & componentType() const override                   \
	{                                                                          \
		return sComponentType();                                               \
	}                                                                          \
	static zn::AComponent * instantiate()                                      \
	{                                                                          \
		return new __name();                                                   \
	}

// Shortcut for behaviours (most likely to be user code)
#define ZN_BEHAVIOUR(__name, __updateOrder) \
	ZN_COMPONENT(__name, zn::CG_BEHAVIOUR, __updateOrder)

namespace zn
{

class AComponent;

// All these IDs refer to inheriting a specific type of Component.
// It can change the way they are handled by the engine
// (ie. cameras and renderers impact the render pipeline, bodies
// are updated after behaviours but before rendering...)
enum ComponentGroup
{
	CG_BEHAVIOUR     = 0,
	CG_RENDERER      = 1,
	CG_COLLIDER      = 2,
	CG_BODY          = 3,
	CG_ANIMATOR      = 4,
	CG_CAMERA        = 5,
	CG_AUDIO_EMITTER = 7
};

// Meta-class of a component
// Note: it is fixed in the final game.
struct ZN_API ComponentType
{
	std::string name;
	u8 group;
//	u8 flags;
	s32 updateOrder; // only used by behaviours
	//dependencies?

	ComponentType(
		const std::string & p_name,
		u8 p_group,
		s32 p_updateOrder=0
	) :
		name(p_name),
		group(p_group),
		updateOrder(p_updateOrder)
	{}

	void print(std::ostream & os) const
	{
		os << "{" << name << ", group:" << (u32)group << ", updateOrder:" << updateOrder << "}";
	}

	// -------------------
	// Factory system
	// -------------------

	// Functions below allow you to register components in the factory system,
	// which can then be created from their name as strings.
	// This is mainly used for serialization.

	// Registers a component type by a name and a function that instantiates this type.
	// You must always use the full name of the class in the C++ way of things.
	static void registerType(const std::string className, std::function<AComponent*()> factory);

	// Creates a new instance of a component from its name.
	// It does the same thing as "new MyComponent()", where className = "MyComponent".
	static AComponent * instantiate(const std::string className);

	// Registers built-in components. Must be called once.
	static void registerEngineComponents();

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED


