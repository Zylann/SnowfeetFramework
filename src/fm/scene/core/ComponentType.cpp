#include <fm/scene/core/ComponentType.hpp>
#include <fm/scene/core/ComponentFactory.hpp>

namespace zn
{

bool ComponentType::is(const ComponentType & other) const
{
	if(ID == 0)
		return false;
	if(ID == other.ID)
	{
		return true;
	}
	else
	{
		ComponentFactory & componentFactory = ComponentFactory::get();
		const ComponentType * baseType = this;

		while(!baseType->baseName.empty())
		{
			baseType = componentFactory.getType(baseType->baseName);
			if(baseType == nullptr)
				return false;
			if(baseType->ID == other.ID)
				return true;
		}
	}

	return false;
}

} // namespace zn


