#ifndef HEADER_ZN_TEXTRENDERER_HPP_INCLUDED
#define HEADER_ZN_TEXTRENDERER_HPP_INCLUDED

#include <fm/proto/graphics/Renderer.hpp>

namespace zn
{

class ZN_API TextRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::TextRenderer, CG_RENDERER, 0)

	TextRenderer() : ARenderer(),
		r_font(nullptr)
	{}

	void setText(std::wstring text) { m_text.setString(text); }
	void setColor(const sf::Color & color) { m_text.setColor(color); }
	void setFont(const std::string & fontName);
	void setCharacterSize(u32 sizePx) { m_text.setCharacterSize(sizePx); }
	void setStyle(u32 styleMask) { m_text.setStyle(styleMask); }

	sf::FloatRect localBounds() const override;

	const std::wstring text() const;
	const sf::Color & color() const { return m_text.getColor(); }
	const sf::Font * font() const { return m_text.getFont(); }
	u32 characterSize() const { return m_text.getCharacterSize(); }
	u32 style() const { return m_text.getStyle(); }

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

	// Note: TextRenderer doesn't supports atlases yet.

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	sf::Text m_text;
	const sf::Font * r_font;

};

} // namespace zn

#endif // HEADER_ZN_TEXTRENDERER_HPP_INCLUDED


