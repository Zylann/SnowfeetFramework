/*
typecheck.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef HEADER_ZN_TYPECHECK_HPP_INCLUDED
#define HEADER_ZN_TYPECHECK_HPP_INCLUDED

#include <typeinfo>
#include <fm/config.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

/// \brief Performs a dynamic_cast with a meaningful error in debug mode,
/// and a static_cast in release.
/// Note: B may be a pointer type.
template <typename B, typename A>
inline B checked_cast(A * a)
{
#ifdef ZN_DEBUG
	// Slow and explicit check
	B castedPointer = dynamic_cast<B>(a);
	if(castedPointer == nullptr)
	{
		// Print an error, and attempt to write runtime types information.
		// Note that typeid(B).name() might not always return meaningful information,
		// as typeid() only has to return different values (whatever they are)
		// for different types, as defined in C++ specs.
		log.err() << "E: Failed to cast pointer ("
			"from {" << (a==nullptr ? "nullptr" : typeid(a).name()) << "} "
			"to {" << typeid(B).name() << "}" << log.endl();
	}
	// Note: no crash here, because debugging would always lead to this file
	// and wouldn't be helpful.
	//assert(castedPointer);
	return castedPointer;
#else
	// Fast
	return static_cast<B>(a);
#endif
}

} // namespace zn

#endif // HEADER_ZN_TYPECHECK_HPP_INCLUDED


