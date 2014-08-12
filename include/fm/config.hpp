/*
config.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef HEADER_ZN_CONFIG_HPP_INCLUDED
#define HEADER_ZN_CONFIG_HPP_INCLUDED

//------------------------------------------------------------------------------
// Switches
// Note: these #defines can either be active here or in your build commands.

// Enables debug code (additionnal checks and messages, may decrease performance)
//#define ZN_DEBUG

// Hint to ease SFML integration
// TODO use SFML_API instead?
#define ZN_SFML

// Enables static compilation config.
//#define ZN_STATIC

// Enables squirrel binding code
#define ZN_SQUIRREL

// For compilation on Windows
//#define ZN_OS_WINDOWS

//------------------------------------------------------------------------------
// Exports

#ifndef ZN_STATIC

	// ZN_API is placed before each element we want to expose to the user of the dynamic library.
	// (Header-only definitions don't need that)

	#ifdef ZN_OS_WINDOWS
		// Windows compilers need specific (and different) keywords for export and import
		#define ZN_API __declspec(dllexport)
	#else // Linux, FreeBSD, Mac OS X
		#if __GNUC__ >= 4
			// GCC 4 has special keywords for showing/hidding symbols,
			// the same keyword is used for both importing and exporting
			#define ZN_API __attribute__ ((__visibility__ ("default")))
		#else
			// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
			#define ZN_API
		#endif
	#endif
#else
	// Static build doesn't need import/export macros
	#define SFML_API_EXPORT
	#define SFML_API_IMPORT
#endif

//------------------------------------------------------------------------------
// C++11 compatibility
#ifdef _MSC_VER
	#define ZN_NOEXCEPT _NOEXCEPT
#else
	#define ZN_NOEXCEPT noexcept
#endif

#endif // HEADER_ZN_CONFIG_HPP_INCLUDED





