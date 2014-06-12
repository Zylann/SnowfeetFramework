#include <fm/reflect/ObjectType.hpp>
#include <fm/reflect/ObjectTypeDatabase.hpp>

namespace zn
{

bool ObjectType::is(const ObjectType & other) const
{
	if(ID == 0)
		return false;
	if(ID == other.ID)
	{
		return true;
	}
	else
	{
		ObjectTypeDatabase & odb = ObjectTypeDatabase::get();
		const ObjectType * baseType = this;

		while(!baseType->baseName.empty())
		{
			baseType = odb.getType(baseType->baseName);
			if(baseType == nullptr)
				return false;
			if(baseType->ID == other.ID)
				return true;
		}
	}

	return false;
}

} // namespace zn

