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

void ACollider::serializeData(JsonBox::Value & o)
{
	AComponent::serializeData(o);
}

void ACollider::unserializeData(JsonBox::Value & o)
{
	AComponent::unserializeData(o);
}

void ACollider::postUnserialize()
{
	AComponent::postUnserialize();
}

} // namespace zn

