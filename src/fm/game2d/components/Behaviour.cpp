#include "Behaviour.hpp"
#include "../Scene.hpp"

namespace zn
{

void ABehaviour::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	owner().scene().registerBehaviour(this);
}

void ABehaviour::onDestroy()
{
	owner().scene().unregisterBehaviour(this);
}

} // namespace zn

