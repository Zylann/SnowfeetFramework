#include <iostream>
#include "test.hpp"
#include "fm/util/typecheck.hpp"

using namespace zn;
using namespace std;

class I
{
	virtual void f() = 0;
};

class A : public I
{
	int c;
	void f() {}
};

class B : public I
{
	int b;
	void f() {}
};

void test_typecheck()
{
	A a;
	B b;

	A * pointerA = &a;
	B * pointerB = &b;
	I * pointerIA = &a;
	A * nullPointer = nullptr;

	std::cout << "Cast from I* (A) to A* (must succeed)" << std::endl;
	checked_cast<A*>(pointerIA);

	std::cout << "Cast from A to B (must fail)" << std::endl;
	checked_cast<B*>(pointerA);

	std::cout << "Cast from nullptr to B (must fail)" << std::endl;
	checked_cast<B*>(nullPointer);

	std::cout << "Done" << std::endl;
}

