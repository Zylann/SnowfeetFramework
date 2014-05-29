#include <fm/scene/physics/Collider.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

void ACollider::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().physics.registerCollider(this);
}

void ACollider::onDestroy()
{
	entity().scene().physics.unregisterCollider(this);
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
