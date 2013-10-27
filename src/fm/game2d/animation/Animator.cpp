#include "Animator.hpp"
#include "../Scene.hpp"

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

