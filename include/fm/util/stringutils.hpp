/*
stringutils.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_STRINGUTILS_HPP_INCLUDED
#define HEADER_ZN_STRINGUTILS_HPP_INCLUDED

#include <string>
#include <fm/types.hpp>

namespace zn
{
// TODO stringutils: use std::to_string and make standard functions
// But MinGW doesn't seems to include it yet...

std::string ZN_API toString(f32 x);
std::string ZN_API toString(f64 x);
std::string ZN_API toString(s32 x);
std::string ZN_API toString(u32 x);
std::string ZN_API toString(bool b);

std::string ZN_API toStringBin(u32 x, u8 n=32);
std::string ZN_API toStringAlnum(s32 x);

s32 ZN_API toInt(std::string s);
void ZN_API toUpper(std::string & str);

// convert a time given in seconds to a time string formated like "hh:mm:ss:ms"
std::string ZN_API toTimeString(f32 s);

bool ZN_API isPrintableChar(u8 c);

std::string ZN_API cropStr(std::string str, u8 c);

inline std::string fileNameWithoutExtension(std::string path)
{
	path = path.substr(0, path.find_first_of("."));
	path = path.substr(path.find_last_of("/\\")+1);
	return path;
}

} // namespace zn

#endif // HEADER_ZN_STRINGUTILS_HPP_INCLUDED


