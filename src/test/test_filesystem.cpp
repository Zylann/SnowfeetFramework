#include <iostream>
#include <fm/system/filesystem.hpp>

#include "test.hpp"

using namespace std;

void test_filesystem()
{
    vector<string> files;
    zn::getFiles("test_data", files, true);
    for(auto it = files.begin(); it != files.end(); ++it)
	{
		cout << "- " << (*it) << endl;
	}
}

