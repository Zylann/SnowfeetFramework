#ifndef HEADER_ZN_MACROS_HPP_INCLUDED
#define HEADER_ZN_MACROS_HPP_INCLUDED

#include <sstream>
#include <fm/config.hpp>

// Place this at the beginning of a code section that must be called once.
// It will raise a failing assertion if the code is called twice.
// Warning: it is designed as global code, not per-object instance.
#define ZN_CALL_ONCE                                                           \
	static bool __callOnce = true;                                             \
	if(__callOnce)                                                             \
		__callOnce = false;                                                    \
	else                                                                       \
		assert(__callOnce)

// Assertion-like call that raises an exception instead of exit the program.
// Usage: ZN_XASSERT(someValue != 0);
#define ZN_XASSERT(__test)                                                     \
	do{                                                                        \
		if(!__test) {                                                          \
			std::stringstream ss;                                              \
			ss << __FILE__ << ", line " << __LINE__ << ": " << #__test;        \
			throw Exception(ss.str());                                         \
		}                                                                      \
	}while(false)

// Logs a message once. Subsequent executions do nothing.
// Usage: ZN_LOG_ONCE(warn, "This is a warning")
// Warning: it is designed as global code, not per-object instance.
#define ZN_LOG_ONCE(__logtype, __msg)                                          \
	do {                                                                       \
		static bool __once = true;                                             \
		if(__once) {                                                           \
			zn::log.__logtype() << __msg << zn::log.endl();                    \
			__once = false;                                                    \
		}                                                                      \
	}while(false)

#endif // MACROS_HPP_INCLUDED

