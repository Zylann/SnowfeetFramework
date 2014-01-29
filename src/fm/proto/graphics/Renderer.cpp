#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/Scene.hpp>
#include <fm/asset/AssetBank.hpp>

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
void ARenderer::setMaterial(Material * newMaterial)
{
	r_material = newMaterial;
}

//------------------------------------------------------------------------------
Material * ARenderer::material() const
{
	return r_material;
}

//------------------------------------------------------------------------------
void ARenderer::serializeData(JsonBox::Value & o)
{
	AComponent::serializeData(o);
	o["drawOrder"] = drawOrder;
	o["material"] = AssetBank::current()->materials.findName(r_material);
}

//------------------------------------------------------------------------------
void ARenderer::unserializeData(JsonBox::Value & o)
{
	AComponent::unserializeData(o);
	drawOrder = o["drawOrder"].getInt();

	std::string materialName = o["material"].getString();
	if(!materialName.empty())
	{
		r_material = AssetBank::current()->materials.get(materialName);
	}
}

//------------------------------------------------------------------------------
void ARenderer::postUnserialize()
{

}

} // namespace zn

