#include <fm/scene/graphics/Renderer.hpp>
#include <fm/scene/core/Scene.hpp>
#include <fm/asset/AssetBank.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::Renderer)

//------------------------------------------------------------------------------
void Renderer::onAdd(Entity * e)
{
	Component::onAdd(e);
	entity().scene().renderSystem.registerRenderer(this);
}

//------------------------------------------------------------------------------
void Renderer::onDestroy()
{
	entity().scene().renderSystem.unregisterRenderer(this);
}

//------------------------------------------------------------------------------
void Renderer::setMaterial(Material * newMaterial)
{
	r_material = newMaterial;
}

//------------------------------------------------------------------------------
void Renderer::setMaterial(std::string name)
{
	setMaterial(AssetBank::current()->materials.get(name));
}

//------------------------------------------------------------------------------
void Renderer::setTexture(const std::string & name)
{
	setTexture(AssetBank::current()->textures.get(name));
}

//------------------------------------------------------------------------------
void Renderer::setAtlas(const std::string & name)
{
	setAtlas(AssetBank::current()->atlases.get(name));
}

//------------------------------------------------------------------------------
Material * Renderer::material() const
{
	return r_material;
}

//------------------------------------------------------------------------------
void Renderer::onAtlasChanged(const TextureAtlas * atlas)
{
	Animator * animator = entity().animator();
	if(animator != nullptr)
	{
		animator->onAtlasChanged(atlas);
	}
}

//------------------------------------------------------------------------------
sf::FloatRect Renderer::bounds() const
{
	sf::FloatRect box = localBounds();
	box = entity().transform.matrix().transformRect(box);
	return box;
}

//------------------------------------------------------------------------------
void Renderer::serializeData(JsonBox::Value & o)
{
	Component::serializeData(o);
	o["drawOrder"] = drawOrder;
	o["material"] = AssetBank::current()->materials.findName(r_material);
}

//------------------------------------------------------------------------------
void Renderer::unserializeData(JsonBox::Value & o)
{
	Component::unserializeData(o);
	drawOrder = o["drawOrder"].getInt();

	std::string materialName = o["material"].getString();
	if(!materialName.empty())
	{
		r_material = AssetBank::current()->materials.get(materialName);
	}
}

//------------------------------------------------------------------------------
void Renderer::postUnserialize()
{

}

} // namespace zn

