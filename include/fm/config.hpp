#ifndef HEADER_ZN_CONFIG_HPP_INCLUDED
#define HEADER_ZN_CONFIG_HPP_INCLUDED

//
// This header defines global build configuration for the whole framework.
// Note: these #defines can either be active here or in your build commands.
//

// Enables debug code (additionnal checks and messages, may decrease performance)
#define ZN_DEBUG

// Hint to ease SFML integration
// TODO use SFML_API instead?
#define ZN_SFML

// Enables DLL exports of the API.
// Enable it if you compile the project as a DLL you want to use.
// Disable this if you included the code in your project instead.
//#define ZN_DLL

// Enables squirrel binding code
#define ZN_SQUIRREL

// DLL options
#ifdef ZN_DLL
	// This is placed before each element we want to expose to the user of the DLL.
	// (Header-only definitions don't need that)
	#define ZN_API __declspec(dllexport)
#else
	// Not creating a DLL: empty macro
	#define ZN_API
#endif

// C++11 compatibility
#ifdef _MSC_VER
	#define ZN_NOEXCEPT _NOEXCEPT
#else
	#define ZN_NOEXCEPT noexcept
#endif

#endif // HEADER_ZN_CONFIG_HPP_INCLUDED

