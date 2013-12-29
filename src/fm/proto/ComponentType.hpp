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

// Shortcut macros

// A behaviour is a component specific to your game, a bit like a script.
// Behaviours are updated before all other components.
// Place this in public in all your complete-type behaviour-components class declarations.
// __name: class name with namespace (ie: myNamespace::MyBehaviour)
#define ZN_BEHAVIOUR(__name)                                                   \
	const ComponentType & componentType() const override                       \
	{                                                                          \
		static ComponentType cmpt(#__name, CG_BEHAVIOUR);                      \
		return cmpt;                                                           \
	}

// This macro is useful if you are extending a engine kind of component.
// Place this in public in all your complete-type components class declarations.
// Components defined this way are the only ones of their group in an entity.
// __name: class name with full namespace
// __group: group from enum ComponentGroup
#define ZN_COMPONENT(__name, __group)                                          \
	const ComponentType & componentType() const override                       \
	{                                                                          \
		static ComponentType cmpt(#__name, __group, CTF_UNIQUE_OF_GROUP);      \
		return cmpt;                                                           \
	}

namespace zn
{

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

enum ComponentTypeFlags
{
	CTF_DEFAULT = 0,
	CTF_UNIQUE_OF_GROUP = 1
};

// Meta-class of a component
// Note: it is fixed in the final game.
struct ComponentType
{
	std::string name;
	u8 group;
	u8 flags;
	u32 updateOrder; // only used by behaviours
	//dependencies?

	ComponentType(
		const std::string & p_name,
		u8 p_group,
		u32 p_updateOrder=0,
		u8 p_flags=CTF_DEFAULT
	) :
		name(p_name),
		group(p_group),
		flags(p_flags),
		updateOrder(p_updateOrder)
	{}

	void print(std::ostream & os)
	{
		os << "{" << name << ", cg:" << (u32)group << ", order:" << updateOrder << "}";
	}

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED


