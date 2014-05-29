/*
ComponentType.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED
#define HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED

#include <fm/types.hpp>
#include <string>
#include <iostream>
#include <functional>
#include <sstream>

// Shortcut macros

// This macro is useful if you are extending a engine kind of component.
// Place this in public in all your components class declarations.
// __name: class name with full namespace (must be unique !)
// __baseName: inherited class name with full namespace
// __updateOrder: update order of the component (only for updatables)
#define ZN_COMPONENT(__name, __baseName, __updateOrder)                        \
	static zn::ComponentType & sComponentType() {                              \
		static zn::ComponentType cmpt(                                         \
			#__name, #__baseName, __updateOrder                                \
		);                                                                     \
		return cmpt;                                                           \
	}                                                                          \
	const zn::ComponentType & componentType() const override {                 \
		return sComponentType();                                               \
	}                                                                          \
	static zn::AComponent * instantiate() {                                    \
		return new __name();                                                   \
	}

#define ZN_ABSTRACT_COMPONENT(__name, __baseName, __updateOrder)               \
	static zn::ComponentType & sComponentType() {                              \
		static zn::ComponentType cmpt(                                         \
			#__name, #__baseName, __updateOrder                                \
		);                                                                     \
		return cmpt;                                                           \
	}                                                                          \
	const zn::ComponentType & componentType() const override {                 \
		return sComponentType();                                               \
	}                                                                          \
	static zn::AComponent * instantiate() {                                    \
		return nullptr;                                                        \
	}

namespace zn
{

class AComponent;

typedef u32 ComponentTypeID;

/// \brief Meta-class of a component
/// \note it is fixed in the final game.
struct ZN_API ComponentType
{
	/// \brief Unique numeric ID of the component.
	/// DO NOT modify (only done on type registration).
	/// The value of IDs may be generated sequentially, starting from 1.
	/// 0 means null type.
	ComponentTypeID ID;

	/// \brief Unique name of the type.
	std::string name;

	/// \brief name of the base component class
	std::string baseName;

	/// \brief Update order (matters for behaviours)
	s32 updateOrder;

	// TODO component dependencies?

	ComponentType(
		const std::string & p_name,
		const std::string & p_baseName,
		s32 p_updateOrder=0
	) :
		ID(0), // null ID, until the type gets registered
		name(p_name),
		baseName(p_baseName),
		updateOrder(p_updateOrder)
	{}

	void print(std::ostream & os) const
	{
		os << "{[" << ID << "]" << name << ", base:" << baseName << ", updateOrder:" << updateOrder << "}";
	}

	std::string toString() const
	{
		std::stringstream ss;
		print(ss);
		return ss.str();
	}

	bool is(const ComponentType & other) const;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENTTYPE_HPP_INCLUDED


