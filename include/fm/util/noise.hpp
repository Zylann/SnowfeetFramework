/*
noise.hpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft framework project.
*/

#ifndef HEADER_ZN_NOISE_HPP_INCLUDED
#define HEADER_ZN_NOISE_HPP_INCLUDED

#include <fm/types.hpp>

namespace zn
{

f32 noise2d(s32 x, s32 y, s32 seed);

f32 noise2dGradient(f32 x, f32 y, s32 seed);

f32 noise2dPerlin(
	f32 x, f32 y, s32 seed,
	s32 octaves, f32 persistence, f32 period);

f32 noise3d(s32 x, s32 y, s32 z, s32 seed);

f32 noise3dGradient(f32 x, f32 y, f32 z, s32 seed);

f32 noise3dPerlin(
	f32 x, f32 y, f32 z, s32 seed,
	s32 octaves, f32 persistence, f32 period);

} // namespace zn


#endif // HEADER_ZN_NOISE_HPP_INCLUDED
