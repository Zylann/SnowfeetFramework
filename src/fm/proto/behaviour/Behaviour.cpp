#include "Behaviour.hpp"
#include "../Scene.hpp"

namespace zn
{

void ABehaviour::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().registerBehaviour(this);
}

void ABehaviour::onDestroy()
{
	entity().scene().unregisterBehaviour(this);
}

} // namespace zn

