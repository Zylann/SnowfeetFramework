#include "TextRenderer.hpp"
#include "../../asset/AssetBank.hpp"
#include "../Entity.hpp"
#include "../../json/json_utils.hpp"

namespace zn
{

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
	states.transform.combine(entity().matrix());
	target.draw(m_text, states);
}

//------------------------------------------------------------------------------
void TextRenderer::serializeData(JsonBox::Value & o)
{
	ARenderer::serializeData(o);

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
	ARenderer::unserializeData(o);

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

