#include <fm/proto/physics/Body.hpp>
#include <fm/proto/Scene.hpp>

namespace zn
{

void Body::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().bodies.add(this);
}

void Body::onDestroy()
{
	entity().scene().bodies.remove(this);
}

} // namespace zn

