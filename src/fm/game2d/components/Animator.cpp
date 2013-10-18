#include "Animator.hpp"
#include "../Scene.hpp"

namespace zn
{

void AAnimator::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	owner().scene().animators.registerComponent(this);
}

void AAnimator::onDestroy()
{
	owner().scene().animators.unregisterComponent(this);
}

} // namespace zn

