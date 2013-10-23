#include <iostream>
#include "ssm/Game.hpp"

using namespace std;

int main()
{
	cout << "Enter main" << endl;

	int exitCode = zn::GameApp::run<ssm::Game>();

	cout << "Exit main" << endl;
	return exitCode;
}



