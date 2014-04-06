#include <iostream>
#include "demo/Game.hpp"
#include "test/test.hpp"

using namespace std;

int main(int argc, char*argv[])
{
	cout << "Enter main" << endl;

	int exitCode = 0;
	exitCode = zn::Application::run<demo::Game>();
	//test_sampledcurve();
	//test_mazeGenerator();

	cout << "Exit main" << endl;
	return exitCode;
}



