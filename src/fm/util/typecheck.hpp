#ifndef HEADER_ZN_TYPECHECK_HPP_INCLUDED
#define HEADER_ZN_TYPECHECK_HPP_INCLUDED

#include <cassert>
#include "../config.hpp"

namespace zn
{

// Performs a dynamic_cast with error crash in debug mode,
// and a static_cast in release.
// Note: B may be a pointer type.
template <typename B, typename A>
inline B checked_cast(A * a)
{
#ifdef ZN_DEBUG
	// Slow and explicit check
	B b = dynamic_cast<B>(a);
	assert(b /*cast*/);
	return b;
#else
	// Fast
	return static_cast<B>(a);
#endif
}

} // namespace zn

#endif // HEADER_ZN_TYPECHECK_HPP_INCLUDED


