/*
interpolation.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft framework project.
*/

#ifndef HEADER_ZN_INTERPOLATION_HPP_INCLUDED
#define HEADER_ZN_INTERPOLATION_HPP_INCLUDED

#include <cmath>
#include <fm/types.hpp>

namespace zn
{
// x in [0,1]
inline f32 smoothCurve(f32 x)
{
	return 6*x*x*x*x*x - 15*x*x*x*x + 10*x*x*x;
}

//
// Linear interpolations
//

inline f32 linearInterpolation(f32 x0, f32 x1, f32 t)
{
	return x0 + (x1 - x0) * t;
}

f32 biLinearInterpolationSmooth(
	const f32 x0y0, const f32 x1y0,
	const f32 x0y1, const f32 x1y1,
	const f32 x, const f32 y);

f32 triLinearInterpolation(
	const f32 v000, const f32 v100, const f32 v010, const f32 v110,
	const f32 v001, const f32 v101, const f32 v011, const f32 v111,
	const f32 x, const f32 y, const f32 z);

//
// Cubic interpolations
//

f32 cubicInterpolation(f32 x0, f32 x1, f32 x2, f32 x3, f32 t);
f32 biCubicInterpolation(f32 x0y[4], f32 x1y[4], f32 x2y[4], f32 x3y[4], f32 x);

} // namespace zn


#endif // HEADER_ZN_INTERPOLATION_HPP_INCLUDED

