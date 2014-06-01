#include <fm/scene/physics/Body.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

void Body::onAdd(Entity * e)
{
	Component::onAdd(e);
	entity().scene().physics.registerBody(this);
}

void Body::onDestroy()
{
	entity().scene().physics.unregisterBody(this);
}

} // namespace zn

