#include "Collider.hpp"
#include "../Scene.hpp"

namespace zn
{

void ACollider::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	owner().scene().colliders.registerComponent(this);
}

void ACollider::onDestroy()
{
	owner().scene().colliders.unregisterComponent(this);
}

} // namespace zn

