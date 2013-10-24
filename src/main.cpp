#include <iostream>
#include "ssm/Game.hpp"

using namespace std;

int main(int argc, char*argv[])
{
	cout << "Enter main" << endl;

	int exitCode = 0;
	exitCode = zn::GameApp::run<ssm::Game>();

	cout << "Exit main" << endl;
	return exitCode;
}



