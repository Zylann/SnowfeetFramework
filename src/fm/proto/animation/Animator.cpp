#include <fm/proto/animation/Animator.hpp>
#include <fm/proto/core/Scene.hpp>

namespace zn
{

void AAnimator::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().animators.add(this);
}

void AAnimator::onDestroy()
{
	entity().scene().animators.remove(this);
}

} // namespace zn

