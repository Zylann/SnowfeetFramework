#include <fm/proto/animation/Animator.hpp>
#include <fm/proto/Scene.hpp>

namespace zn
{

void AAnimator::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().animators.registerComponent(this);
}

void AAnimator::onDestroy()
{
	entity().scene().animators.unregisterComponent(this);
}

} // namespace zn

