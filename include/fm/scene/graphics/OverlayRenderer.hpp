#ifndef HEADER_ZN_OVERLAYRENDERER_HPP_INCLUDED
#define HEADER_ZN_OVERLAYRENDERER_HPP_INCLUDED

#include <fm/scene/graphics/Renderer.hpp>

namespace zn
{

/// \brief Draws a quad matching the whole screen regardless of the transform.
class ZN_API OverlayRenderer : public Renderer
{
public:

	ZN_COMPONENT(zn::OverlayRenderer, zn::Renderer, 0)

	OverlayRenderer();

	void setTexture(const sf::Texture * texture);
	void setColor(sf::Color color);

	sf::FloatRect localBounds() const override;

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	const sf::Texture * r_texture;
	sf::Color m_color;

};

} // namespace zn

#endif // HEADER_ZN_OVERLAYRENDERER_HPP_INCLUDED

