#include <fm/proto/physics/Collider.hpp>
#include <fm/proto/Scene.hpp>

namespace zn
{

void ACollider::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().colliders.add(this);
}

void ACollider::onDestroy()
{
	entity().scene().colliders.remove(this);
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

