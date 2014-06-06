#include <fm/scene/animation/Animator.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::Animator)

void Animator::onAdd(Entity * e)
{
	Component::onAdd(e);
	entity().scene().animators.add(this);
}

void Animator::onDestroy()
{
	entity().scene().animators.remove(this);
}

} // namespace zn

