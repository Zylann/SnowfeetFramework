#include <fm/squirrel/squirrel_utils.hpp>
#include <fm/util/Vector2_Squirrel.hpp>

#ifdef SQUNICODE
	#define scvprintf vfwprintf
#else
	#define scvprintf vfprintf
#endif

namespace zn
{
namespace squirrel
{

void printfunc(HSQUIRRELVM v, const SQChar *s,...)
{
	// TODO use the Log
	va_list vl;
	va_start(vl, s);
	scvprintf(stdout, s, vl);
	va_end(vl);
}

void errorfunc(HSQUIRRELVM v, const SQChar *s,...)
{
	// TODO use the Log
	va_list vl;
	va_start(vl, s);
	scvprintf(stderr, s, vl);
	va_end(vl);
}

void registerFramework(HSQUIRRELVM vm)
{
	// registers the default error handlers
	sqstd_seterrorhandlers(vm);

	// sets the print function
	sq_setprintfunc(vm, printfunc, errorfunc);

	registerVector2<f32>(vm, "Vector2f");
	registerVector2<f32>(vm, "Vector2i");
}

} // namespace squirrel
} // namespace zn



