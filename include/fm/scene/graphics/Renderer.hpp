#ifndef HEADER_ZN_RENDERER_HPP_INCLUDED
#define HEADER_ZN_RENDERER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <fm/scene/core/Component.hpp>
#include <fm/asset/TextureAtlas.hpp>
#include <fm/asset/Material.hpp>

namespace zn
{

// Base component responsible for drawing things.
class ZN_API ARenderer : public AComponent, public sf::Drawable
{
public:

	ZN_ABSTRACT_COMPONENT(zn::ARenderer, zn::AComponent, 0)

	void onAdd(Entity * e);
	void onDestroy();

	/// \brief Returns the axis-aligned bounding box of what's being rendered in local space
	virtual sf::FloatRect localBounds() const = 0;

	/// \brief returns the axis-aligned bounding box of what's being rendered in world space
	sf::FloatRect bounds() const;

	// Serialization
	virtual void serializeData(JsonBox::Value & o) override;
	virtual void unserializeData(JsonBox::Value & o) override;
	virtual void postUnserialize() override;

	// Sets the material for this renderer.
	// It allows you to configure visual effects on the entity.
	// Note that it only references it, so if you created the material yourself,
	// you have to delete it after use. (except for materials stored in AssetBank).
	// If the material is null, the default rendering will occur.
	void setMaterial(Material * newMaterial);

	// Gets the current material of this renderer, if any
	Material * material() const;

	virtual void setTexture(const sf::Texture * texture) {}
	virtual const sf::Texture * texture() const { return nullptr; }

	// Sets the atlas this renderer should use. Note that the atlas may or may
	// not be used depending of the type of renderer.
	virtual void setAtlas(const TextureAtlas * atlas) {}

	// Gets the current atlas used by this renderer, if any
	virtual const TextureAtlas * atlas() const { return nullptr; }

	// Sets the rectangle within the texture to render.
	// This function is used by animators to switch frames of a sprite.
	virtual void setTextureRect(const sf::IntRect & rect) {}

	// Defines the draw order of the renderer.
	// A high value means later (front), a lower value means earlier (back).
	// Renderers are sorted on each frame, so this value can be adjusted anytime you want.
	s32 drawOrder;

protected:

	ARenderer() : AComponent(),
		drawOrder(0),
		r_material(nullptr)
	{}

	void onAtlasChanged(const TextureAtlas * atlas);

	Material * r_material;

};

} // namespace zn

#endif // HEADER_ZN_RENDERER_HPP_INCLUDED

