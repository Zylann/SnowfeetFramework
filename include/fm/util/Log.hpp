/*
Log.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_LOG_HPP_INCLUDED
#define HEADER_ZN_LOG_HPP_INCLUDED

#include <iostream>
#include <string>
#include <fstream>

#define log_error zn::log << "E: "
#define log_warning zn::log << "W: "
#define log_info zn::log << "I: "
#define log_debug zn::log << "D: "

namespace zn
{

// Simple stream wrapper providing file or console output
class Log
{
public:

	bool openFile(const std::string & fpath)
	{
		m_file.open(fpath.c_str());
		if(!m_file.good())
		{
			std::cout << "E: Log: failed to open file \"" << fpath << '"' << std::endl;
			return false;
		}
		return true;
	}

	void closeFile()
	{
		m_file.close();
	}

	template <typename T>
	Log & operator<<(T const & a)
	{
		if(m_file)
		{
			m_file << a;
		}
#ifdef ZN_DEBUG
		std::cout << a;
#endif
		return *this;
	}

private:

	std::ofstream m_file;

};

extern Log log;

} // namespace zn

#endif // HEADER_ZN_LOG_HPP_INCLUDED



