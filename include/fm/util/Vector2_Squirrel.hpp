/*
Vector2_Squirrel.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef HEADER_ZN_VECTOR2_SQUIRREL_HPP_INCLUDED
#define HEADER_ZN_VECTOR2_SQUIRREL_HPP_INCLUDED

#include <sqrat.h>
#include <fm/util/Vector2.hpp>

namespace zn {
namespace squirrel {

template <typename T>
void registerVector2(HSQUIRRELVM vm, const char * className)
{
	using namespace Sqrat;

	// TODO in squirrel, this should be implemented as a simple Vector2
	// coded with userdata values.

	RootTable(vm).Bind(className, Class<Vector2<T>>(vm)
		.Var("x", &Vector2<T>::x)
		.Var("y", &Vector2<T>::y)
//		.Overload<void(*)(T,T)>("set", &Vector2<T>::set)
//		.Overload<void(*)(Vector2<T>)>("set", &Vector2<T>::set)
	);
}

} // namespace squirrel
} // namespace zn

#endif // HEADER_ZN_VECTOR2_SQUIRREL_HPP_INCLUDED


