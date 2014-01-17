#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/Scene.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void ARenderer::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().renderers.registerComponent(this);
}

//------------------------------------------------------------------------------
void ARenderer::onDestroy()
{
	entity().scene().renderers.unregisterComponent(this);
}

//------------------------------------------------------------------------------
void ARenderer::serializeData(JsonBox::Value & o)
{
	AComponent::serializeData(o);
	o["drawOrder"] = drawOrder;
}

//------------------------------------------------------------------------------
void ARenderer::unserializeData(JsonBox::Value & o)
{
	AComponent::unserializeData(o);
	drawOrder = o["drawOrder"].getInt();
}

//------------------------------------------------------------------------------
void ARenderer::postUnserialize()
{

}

} // namespace zn

