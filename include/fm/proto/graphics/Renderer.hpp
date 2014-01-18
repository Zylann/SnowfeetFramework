#ifndef HEADER_ZN_RENDERER_HPP_INCLUDED
#define HEADER_ZN_RENDERER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include "../Component.hpp"
#include "../../asset/TextureAtlas.hpp"

namespace zn
{

class ZN_API ARenderer : public AComponent, public sf::Drawable
{
public:

	void onAdd(Entity * e);
	void onDestroy();

	// Returns the axis-aligned bounding box of what's being rendered in local space
	virtual sf::FloatRect localBounds() const = 0;

	// Serialization
	virtual void serializeData(JsonBox::Value & o) override;
	virtual void unserializeData(JsonBox::Value & o) override;
	virtual void postUnserialize() override;

protected:

	ARenderer() : AComponent(), drawOrder(0) {}

public:

	virtual const TextureAtlas * atlas() const { return nullptr; }
	virtual void setAtlas(const TextureAtlas * atlas) {}
	virtual void setTextureRect(const sf::IntRect & rect) {}

	// Defines the draw order of the renderer.
	// A high value means later (front), a lower value means earlier (back).
	// Renderers are sorted on each frame, so this value can be adjusted anytime you want.
	s32 drawOrder;

};

} // namespace zn

#endif // HEADER_ZN_RENDERER_HPP_INCLUDED

