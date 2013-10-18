#ifndef HEADER_ZN_RENDERER_HPP_INCLUDED
#define HEADER_ZN_RENDERER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include "Component.hpp"
#include "../../asset/TextureAtlas.hpp"

namespace zn
{

class ARenderer : public AComponent, public sf::Drawable
{
public:

	void onAdd(Entity * e);
	void onDestroy();

protected:

	ARenderer() : AComponent() {}

public:

	virtual const TextureAtlas * atlas() { return nullptr; }
	virtual void setAtlas(const TextureAtlas * atlas) {}
	virtual void setTextureRect(const sf::IntRect & rect) {}

	// TODO draw order

};

} // namespace zn

#endif // HEADER_ZN_RENDERER_HPP_INCLUDED

