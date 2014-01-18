#ifndef MACROS_HPP_INCLUDED
#define MACROS_HPP_INCLUDED

#include <sstream>
#include <fm/config.hpp>

// Place this at the beginning of a code section that may be called twice.
// It will raise a failing assertion if the code is called twice.
#define ZN_CALL_ONCE                                                           \
	static bool __callOnce = true;                                             \
	if(__callOnce)                                                             \
		__callOnce = false;                                                    \
	else                                                                       \
		assert(__callOnce)

// Assertion-like call that raises an exception instead of exit the program.
#define xassert(__test)                                                        \
	if(!__test) {                                                              \
		std::stringstream ss;                                                  \
		ss << __FILE__ << ", line " << __LINE__ << ": " << #__test;            \
		throw Exception(ss.str());                                             \
	}

#endif // MACROS_HPP_INCLUDED

