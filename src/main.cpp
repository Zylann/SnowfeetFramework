#include <iostream>
#include "demo/Game.hpp"
//#include "test/test.hpp"

// TODO compile zlib as a separate lib
// TODO compile JsonBox as a separate lib (the code seems not prepared for this)
// TODO separate projects for the engine lib, the demo and tests
// TODO find a name for the engine. "Plane"? ("proto" is a working title !)

using namespace std;

int main(int argc, char*argv[])
{
	cout << "Enter main" << endl;

	int exitCode = 0;
	exitCode = zn::GameApp::run<demo::Game>();
//	test_autotiler();

	cout << "Exit main" << endl;
	return exitCode;
}



