#include "squirrel_utils.hpp"

#ifdef SQUNICODE
	#define scvprintf vfwprintf
#else
	#define scvprintf vfprintf
#endif

#include <fm/util/Vector2_Squirrel.hpp>

namespace zn {
namespace squirrel {

void printfunc(HSQUIRRELVM v, const SQChar *s,...)
{
	va_list vl;
	va_start(vl, s);
	scvprintf(stdout, s, vl);
	va_end(vl);
}

void errorfunc(HSQUIRRELVM v, const SQChar *s,...)
{
	va_list vl;
	va_start(vl, s);
	scvprintf(stderr, s, vl);
	va_end(vl);
}

void registerFramework(HSQUIRRELVM vm)
{
	registerVector2<f32>(vm, "Vector2");
}


} // namespace squirrel
} // namespace zn



