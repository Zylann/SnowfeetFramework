#include "Body.hpp"
#include "../Scene.hpp"

namespace zn
{

void Body::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	owner().scene().bodies.registerComponent(this);
}

void Body::onDestroy()
{
	owner().scene().bodies.unregisterComponent(this);
}

} // namespace zn

