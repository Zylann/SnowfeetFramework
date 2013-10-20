#include "Body.hpp"
#include "../Scene.hpp"

namespace zn
{

void Body::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().bodies.registerComponent(this);
}

void Body::onDestroy()
{
	entity().scene().bodies.unregisterComponent(this);
}

} // namespace zn

