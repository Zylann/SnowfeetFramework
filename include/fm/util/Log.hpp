/*
Log.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef HEADER_ZN_LOG_HPP_INCLUDED
#define HEADER_ZN_LOG_HPP_INCLUDED

#include <iostream>
#include <string>
#include <fstream>

#include <fm/types.hpp>
#include <fm/util/NonCopyable.hpp>

namespace zn
{

/// \brief Simple stream wrapper providing file and/or console output
class ZN_API Log : public NonCopyable
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
		m_consoleOutputFlags(M_WARNING | M_ERROR | M_DEBUG)
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

	/// \brief Outputs more lines to a previous log.* call,
	/// making them appear as an easy-to-spot block.
	/// Keeps last message type.
	/// \return Log itself for chaining.
	inline Log & more()
	{
		return (*this) << "| ";
	}

	/// \brief Returns the end-of-line character used to mark the end of a log message.
	/// \note use: log.info() << "Message" << log.endl();
	inline char endl()
	{
		return '\n';
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
			if(m_messageType == M_ERROR)
			{
				std::cerr << a;
			}
			else
			{
				std::cout << a;
			}
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
extern ZN_API Log log;

} // namespace zn

#endif // HEADER_ZN_LOG_HPP_INCLUDED



