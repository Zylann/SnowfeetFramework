#ifndef HEADER_ZN_CONFIG_HPP_INCLUDED
#define HEADER_ZN_CONFIG_HPP_INCLUDED

//
// This header defines global build configuration for the whole framework.
//

// Note: these #defines can either be active here or in your build commands.

// Enables debug code (additionnal checks and messages, may decrease performance)
#define ZN_DEBUG

// Enables editor integration (not implemented yet)
//#define ZN_EDITOR

// Hint to ease SFML integration
// TODO use SFML_API instead?
#define ZN_SFML

// Hint for debug profiling. May decrease performance.
#define ZN_PROFILE

// Hint to maximize C++0x compatibility
//#define ZN_CPP0X

// Enables DLL exports of the API.
// Enable it if you compile the project as a DLL you want to use.
// Disable this if you included the code in your project instead.
//#define ZN_DLL

// DLL options
#ifdef ZN_DLL
	// This is placed before each element we want to expose to the user of the DLL.
	// (Header-only definitions don't need that)
	#define ZN_API __declspec(dllexport)
#else
	// Not creating a DLL: empty macro
	#define ZN_API
#endif

#endif // HEADER_ZN_CONFIG_HPP_INCLUDED

