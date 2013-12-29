#ifndef MACROS_HPP_INCLUDED
#define MACROS_HPP_INCLUDED

#include "../config.hpp"

#define ZN_CALL_ONCE                                                           \
	static bool __callOnce = true;                                             \
	if(__callOnce)                                                             \
		__callOnce = false;                                                    \
	else                                                                       \
		assert(__callOnce)

#endif // MACROS_HPP_INCLUDED

