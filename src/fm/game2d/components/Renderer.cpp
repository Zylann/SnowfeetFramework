#include "Renderer.hpp"
#include "../Scene.hpp"

namespace zn
{

void ARenderer::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	owner().scene().renderers.registerComponent(this);
}

void ARenderer::onDestroy()
{
	owner().scene().renderers.unregisterComponent(this);
}

} // namespace zn

