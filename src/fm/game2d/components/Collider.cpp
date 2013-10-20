#include "Collider.hpp"
#include "../Scene.hpp"

namespace zn
{

void ACollider::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().colliders.registerComponent(this);
}

void ACollider::onDestroy()
{
	entity().scene().colliders.unregisterComponent(this);
}

} // namespace zn

