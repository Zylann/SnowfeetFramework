/*
stringutils.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_STRINGUTILS_HPP_INCLUDED
#define HEADER_ZN_STRINGUTILS_HPP_INCLUDED

#include <string>

namespace zn
{
// TODO stringutils: use std::to_string and make standard functions
// But MinGW doesn't seems to include it for now...

std::string toString(float x);
std::string toString(double x);
std::string toString(int x);
std::string toString(unsigned int x);
std::string toString(bool b);

std::string toStringBin(unsigned int x, unsigned char n=32);
std::string toStringAlnum(int x);

int toInt(std::string s);
void toUpper(std::string & str);

// convert a time given in seconds to a time string formated like "hh:mm:ss:ms"
std::string toTimeString(float s);

bool isPrintableChar(char c);

std::string cropStr(std::string str, char c);


} // namespace zn

#endif // HEADER_ZN_STRINGUTILS_HPP_INCLUDED


