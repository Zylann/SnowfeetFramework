#include <fm/scene/graphics/TextRenderer.hpp>
#include <fm/asset/AssetBank.hpp>
#include <fm/scene/core/Entity.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

ZN_OBJECT_IMPL(zn::TextRenderer)

//------------------------------------------------------------------------------
const std::wstring TextRenderer::text() const
{
	return m_text.getString().toWideString();
}

//------------------------------------------------------------------------------
void TextRenderer::setFont(const std::string & fontName)
{
	r_font = AssetBank::current()->fonts.get(fontName);
	if(r_font != nullptr)
	{
		m_text.setFont(*r_font);
	}
}

//------------------------------------------------------------------------------
void TextRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.combine(entity().transform.matrix());
	target.draw(m_text, states);
}

//------------------------------------------------------------------------------
sf::FloatRect TextRenderer::localBounds() const
{
	// Note: currently, the internal text object has an internal (unusued) transform.
	// this is the global bounds from this transform, not the one from the entity hierarchy.
	// (An SDL switch may fix this overhead one day :D)
	return m_text.getGlobalBounds();
}

//------------------------------------------------------------------------------
void TextRenderer::serializeData(JsonBox::Value & o)
{
	Renderer::serializeData(o);

	std::string fontName = AssetBank::current()->fonts.findName(r_font);
	o["font"] = fontName;

	std::string utf8 = m_text.getString();
	o["text"] = utf8;

	zn::serialize(o, color());
	o["characterSize"] = (s32)characterSize();

	o["style"] = (s32)style();
}

//------------------------------------------------------------------------------
void TextRenderer::unserializeData(JsonBox::Value & o)
{
	Renderer::unserializeData(o);

	std::string fontName = o["font"].getString();
	if(!fontName.empty())
	{
		setFont(fontName);
	}

	m_text.setString(o["text"].getString());

	sf::Color clr;
	zn::unserialize(o["color"], clr);
	setColor(clr);

	setCharacterSize(o["characterSize"].getInt());

	setStyle(o["style"].getInt());
}

//------------------------------------------------------------------------------
void TextRenderer::postUnserialize()
{

}

} // namespace zn

