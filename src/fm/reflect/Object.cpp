#include <fm/reflect/Object.hpp>

namespace zn
{

// Static
zn::ObjectType & Object::sObjectType()
{
	static zn::ObjectType t("zn::Object", "");
	return t;
}


} // namespace zn

