#include <iostream>
#include "fm/util/stringutils.hpp"
#include "test.hpp"

void test_stringutils()
{
	std::cout << "Test filenameWithoutExtension:" << std::endl;
	std::cout << zn::fileNameWithoutExtension("test") << std::endl;
	std::cout << zn::fileNameWithoutExtension("/test") << std::endl;
	std::cout << zn::fileNameWithoutExtension("test.x") << std::endl;
	std::cout << zn::fileNameWithoutExtension("hello/lol/test.x") << std::endl;
	std::cout << zn::fileNameWithoutExtension("hello/lol/test.x.y") << std::endl;

}

