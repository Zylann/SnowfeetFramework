#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#ifndef WIN32
	#include <sys/types.h>
#endif

#include <fm/system/filesystem.hpp>

// Note: this file compiles better with GCC (MinGW under windows).
// There is a port of dirent for MSVC to ease its use under Visual Studio.

namespace zn
{

bool getFiles(const std::string & folder, std::vector<std::string> & filePaths, bool recursive)
{
	// Open the directory
	DIR * dir = opendir(folder.c_str());
	if(!dir)
	{
		perror("E: Couldn't open directory. Cause");
		return false;
	}

	struct stat st;

	dirent * e = readdir(dir);
	if(e)
	{
		do
		{
			// Do not explore the two special directories "." and ".."
			if(strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
			{
				continue;
			}

			// Get file status
			std::string path = folder + '/' + e->d_name;
			stat(path.c_str(), &st);

			// Is it a directory?
			if(S_ISDIR(st.st_mode))
			{
				if(recursive)
				{
					// Explore subdirectory
					//std::cout << "D - " << path << std::endl;
					getFiles(path, filePaths, true);
				}
			}
			else
			{
				// Add file to list
				//std::cout << "F - " << path << std::endl;
				filePaths.push_back(path);
			}

		} while((e = readdir(dir))); // while there is a file to read
	}

	// Close the directory
	if(closedir(dir) == -1)
	{
		perror("E: An error occurred closing the directory. Cause");
		// Note: the function must return true if it got the files.
	}

	return true;
}

} // namespace zn





