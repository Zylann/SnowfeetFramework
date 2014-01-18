/*
stringutils.cpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft project.
*/

#include <sstream>
#include <cmath>

#include <fm/util/stringutils.hpp>

namespace zn
{
// TODO stringutils: may be optimized

std::string toString(f32 x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

std::string toString(f64 x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

std::string toString(s32 x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

std::string toString(u32 x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

std::string toString(bool b)
{
	return b ? "true" : "false";
}

std::string toStringBin(u32 x, u8 n)
{
	u32 mask = 1 << (n-1);
	std::string str;

	for(u8 i = 0; i < n; i++, mask = mask >> 1)
	{
		if(x & mask)
			str += "1";
		else
			str += "0";
	}
	return str;
}

void toUpper(std::string & str)
{
	for(u32 i = 0; i < str.size(); i++)
		str[i] = toupper(str[i]);
}

s32 toInt(std::string s)
{
	std::stringstream ss(s);
	s32 i;
	ss >> i;
	return i;
}

std::string toTimeString(f32 s)
{
	s32 min = s / 60;
	s32 hr = min / 60;
	min = min % 60;
	s32 sec = ((s32)s) % 60;
	s32 ms = 1000.0 * (s - floor(s));

	std::string s_hr;
	std::string s_min;
	std::string s_sec;
	std::string s_ms;

	if(hr == 0)
		s_hr = "00";
	else if(hr < 10)
		s_hr = "0" + toString(hr);
	else
		s_hr = toString(hr);

	if(min == 0)
		s_min = "00";
	else if(min < 10)
		s_min = "0" + toString(min);
	else
		s_min = toString(min);

	if(sec == 0)
		s_sec = "00";
	else if(sec < 10)
		s_sec = "0" + toString(sec);
	else
		s_sec = toString(sec);

	if(ms == 0)
		s_ms = "000";
	else if(ms < 10)
		s_ms = "00" + toString(ms);
	else if(ms < 100)
		s_ms = "0" + toString(ms);
	else
		s_ms = toString(ms);

	return s_hr + ":" + s_min + ":" + s_sec + ":" + s_ms;
}

bool isPrintableChar(u8 c)
{
	return (c >= 32 && c <= 126); // without the char(13) which is '\n'
}

std::string cropStr(std::string str, u8 c)
{
	if(str.empty())
		return str;

	if(str.size() == 1)
	{
		if(str[0] == c)
			return "";
		return str;
	}

	u32 i = str.find(c);
	if(i != std::string::npos)
		str = str.substr(i + 1);

	i = str.find(c);
	if(i != std::string::npos)
		str = str.substr(0, i);

	return str;
}

} // namespace zn


