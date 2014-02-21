#ifndef HEADER_ZN_FILESYSTEM_HPP_INCLUDED
#define HEADER_ZN_FILESYSTEM_HPP_INCLUDED

// Simple and lightweight filesystem functions.

#include <string>
#include <vector>
#include <fm/config.hpp>

namespace zn
{

/// \brief Searches the contents of a directory for files, and add their path to a vector.
/// \param folder: the folder to search
/// \param filePaths: the vector where to put the file paths
/// \param recursive: if set to true, all subdirectories will be fetched too.
/// \return False if an error occured (like unexisting folder), true if the files were successfully collected.
bool ZN_API getFiles(const std::string & folder, std::vector<std::string> & filePaths, bool recursive=false);

} // namespace zn

#endif // HEADER_ZN_FILESYSTEM_HPP_INCLUDED

