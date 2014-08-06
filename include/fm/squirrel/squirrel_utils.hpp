#ifndef HEADER_ZN_SQUIRREL_UTILS_HPP_INCLUDED
#define HEADER_ZN_SQUIRREL_UTILS_HPP_INCLUDED

#include <stdarg.h>
#include <stdio.h>

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>
#include <sqrat.h>

#ifdef _MSC_VER
	#pragma comment (lib, "squirrel.lib")
	#pragma comment (lib, "sqstdlib.lib")
#endif

namespace zn {
namespace squirrel {

void printFunc(HSQUIRRELVM vm, const SQChar *s,...);
void errorFunc(HSQUIRRELVM vm, const SQChar *s,...);

/// \brief Registers the whole framework to Squirrel
void registerFramework(HSQUIRRELVM vm);

} // namespace squirrel
} // namespace zn

#endif // HEADER_ZN_SQUIRREL_UTILS_HPP_INCLUDED




