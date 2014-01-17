#include <fm/proto/behaviour/Behaviour.hpp>
#include <fm/proto/Scene.hpp>

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

