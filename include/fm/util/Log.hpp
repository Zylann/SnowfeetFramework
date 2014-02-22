/*
Log.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the Plane framework project.
*/

#ifndef HEADER_ZN_LOG_HPP_INCLUDED
#define HEADER_ZN_LOG_HPP_INCLUDED

#include <iostream>
#include <string>
#include <fstream>

#include <fm/types.hpp>

namespace zn
{

/// \brief Simple stream wrapper providing file and/or console output
class Log
{
public:

	enum MessageTypeMask
	{
		M_DEBUG     = 1,
		M_INFO      = 1 << 1,
		M_WARNING   = 1 << 2,
		M_ERROR     = 1 << 3,
		M_ALL       = M_DEBUG | M_INFO | M_WARNING | M_ERROR,
		M_NONE      = 0
	};

	Log() :
		m_messageType(M_INFO),
		m_fileOutputFlags(M_ALL),
#ifdef ZN_DEBUG
		m_consoleOutputFlags(M_ALL)
#else
		m_consoleOutputFlags(M_WARNING | M_ERROR)
#endif
	{}

	/// \brief Sets which types of messages must be written to the file output.
	/// \param flags: bitmask where set flags activate a type of output.
	/// \see Log::MessageTypeMask
	void setFileOutputFlags(u32 flags)
	{
		m_fileOutputFlags = flags;
	}

	/// \brief Sets which types of messages must be written to the console output.
	/// \param flags: bitmask where set flags activate a type of output.
	/// \see Log::MessageTypeMask
	void setConsoleOutputFlags(u32 flags)
	{
		m_consoleOutputFlags = flags;
	}

	/// \brief Sets which file will be used to output log messages.
	/// \param fpath: path to the file to open
	/// \return true if the file was successuflly open, false otherwise
	bool openFile(const std::string & fpath);

	/// \brief Closes the file used to output log messages.
	/// \note This also disables outputting messages to a file unless openFile() gets called again.
	void closeFile()
	{
		m_file.close();
	}

	/// \brief Outputs a debug message.
	/// \return Log itself for chaining on the same line.
	inline Log & debug()
	{
		m_messageType = M_DEBUG;
		return (*this) << "D: ";
	}

	/// \brief Outputs an information message.
	/// \return Log itself for chaining on the same line.
	inline Log & info()
	{
		m_messageType = M_INFO;
		return (*this) << "I: ";
	}

	/// \brief Outputs a warning message.
	/// \return Log itself for chaining on the same line.
	inline Log & warn()
	{
		m_messageType = M_WARNING;
		return (*this) << "W: ";
	}

	/// \brief Outputs an error message.
	/// \return Log itself for chaining on the same line.
	inline Log & err()
	{
		m_messageType = M_DEBUG;
		return (*this) << "E: ";
	}

	/// \brief Marks the end of the current log message.
	/// You can use it at the end of your message like std::endl :
	/// log.info() << "your message" << log.endl();
	/// \return Log itself for chaining calls.
	/// \note Outputting the log itself in a message will do nothing.
	inline Log & endl()
	{
		(*this) << (char)'\n';
		return (*this);
	}

	/// \brief Outputs a message using the current message type.
	/// \note It is recommended to use specialized output functions instead,
	/// because each of them end up calling this one.
	/// \param a: object to print, like with std::cout.
	/// \return Log itself for chaining on the same line.
	template <typename T>
	Log & operator<<(T const & a)
	{
		if(m_file && (m_messageType & m_fileOutputFlags))
		{
			m_file << a;
		}

		if(m_consoleOutputFlags & m_consoleOutputFlags)
		{
			std::cout << a;
		}

		return *this;
	}

	/// \brief Operator overload that does nothing for the Log object itself.
	/// \return Log itself for chaining calls.
	Log & operator<<(Log const & a)
	{
		return *this;
	}

private:

	/// \brief File where messages can be output.
	std::ofstream m_file;

	/// \brief Type of the last message.
	MessageTypeMask m_messageType;

	/// \brief Bitmask storing which messages to output to the file.
	u8 m_fileOutputFlags;

	/// \brief Bitmask storing which messages to output to the console.
	u8 m_consoleOutputFlags;

};

/// \brief Global log variable, for convenience. Behaves like std::cout.
/// \note It is not assumed to exist in Log objects, so you can create other instances without problems.
extern Log log;

} // namespace zn

#endif // HEADER_ZN_LOG_HPP_INCLUDED



