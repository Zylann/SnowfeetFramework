/*
exception.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework.
*/

#ifndef HEADER_ZN_EXCEPTION_HPP_INCLUDED
#define HEADER_ZN_EXCEPTION_HPP_INCLUDED

#include <string>
#include <exception>

namespace zn
{

class Exception : public std::exception
{
public:

	Exception(const std::string msg)
	{
		m_msg = msg;
	}

	const char * what() const noexcept override { return m_msg.c_str(); }

private:

	std::string m_msg;
};

} // namespace zn

#endif // HEADER_ZN_EXCEPTION_HPP_INCLUDED


