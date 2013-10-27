#ifndef HEADER_ZN_RENDERER_HPP_INCLUDED
#define HEADER_ZN_RENDERER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include "../Component.hpp"
#include "../../asset/TextureAtlas.hpp"

namespace zn
{

class ARenderer : public AComponent, public sf::Drawable
{
public:

	void onAdd(Entity * e);
	void onDestroy();

protected:

	ARenderer() : AComponent(), drawOrder(0) {}

public:

	virtual const TextureAtlas * atlas() { return nullptr; }
	virtual void setAtlas(const TextureAtlas * atlas) {}
	virtual void setTextureRect(const sf::IntRect & rect) {}

	// Defines the draw order of the renderer.
	// A high value means later (front), a lower value means earlier (back).
	// Renderers are sorted on each frame, so this value can be adjusted anytime you want.
	s32 drawOrder;

};

} // namespace zn

#endif // HEADER_ZN_RENDERER_HPP_INCLUDED

