#include <fm/scene/physics/Collider.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::Collider)

void Collider::onAdd(Entity * e)
{
	Component::onAdd(e);
	entity().scene().physics.registerCollider(this);
}

void Collider::onDestroy()
{
	entity().scene().physics.unregisterCollider(this);
}

void Collider::serializeData(JsonBox::Value & o)
{
	Component::serializeData(o);
}

void Collider::unserializeData(JsonBox::Value & o)
{
	Component::unserializeData(o);
}

void Collider::postUnserialize()
{
	Component::postUnserialize();
}

} // namespace zn

