#include "squirrel_utils.hpp"

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

SQInteger registerGlobalFunction(HSQUIRRELVM vm, SQFUNCTION f, const char *fname)
{
	sq_pushroottable(vm);
	sq_pushstring(vm, fname, -1);
	sq_newclosure(vm, f, 0); //create a new function
	sq_createslot(vm, -3);
	sq_pop(vm, 1); //pops the root table
	return 0;
}

void call(HSQUIRRELVM vm, const SQChar *fname)
{
	SQInteger top = sq_gettop(vm); //saves the stack size before the call
	sq_pushroottable(vm); //pushes the global table
	sq_pushstring(vm, fname, -1);

	if(SQ_SUCCEEDED(sq_get(vm,-2)))
	{
		//gets the field 'foo' from the global table
		sq_pushroottable(vm); //push the 'this' (in this case is the global table)
		sq_call(vm, 1, SQFalse, SQTrue); //calls the function
	}

	sq_settop(vm,top); //restores the original stack size
}

void call(HSQUIRRELVM vm, const SQChar *fname, int n,float f,const SQChar *s)
{
	SQInteger top = sq_gettop(vm); //saves the stack size before the call
	sq_pushroottable(vm); //pushes the global table
	sq_pushstring(vm, fname, -1);

	if(SQ_SUCCEEDED(sq_get(vm,-2)))
	{
		//gets the field 'foo' from the global table
		sq_pushroottable(vm); //push the 'this' (in this case is the global table)
		sq_pushinteger(vm,n);
		sq_pushfloat(vm,f);
		sq_pushstring(vm,s,-1);
		sq_call(vm,4,SQFalse,SQTrue); //calls the function
	}

	sq_settop(vm,top); //restores the original stack size
}

} // namespace squirrel
} // namespace zn



