#include "Renderer.hpp"
#include "../Scene.hpp"

namespace zn
{

void ARenderer::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().renderers.registerComponent(this);
}

void ARenderer::onDestroy()
{
	entity().scene().renderers.unregisterComponent(this);
}

} // namespace zn

