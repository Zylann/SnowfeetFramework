#include <fm/proto/gui/Anchor.hpp>

namespace zn
{

//------------------------------------------------------------------------------
bool Anchor::onEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		apply();
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------------
void Anchor::setAlign(sf::Vector2f align)
{
	align.x = math::clamp(align.x, -1.f, 1.f);
	align.y = math::clamp(align.y, -1.f, 1.f);
	m_align = align;
	apply();
}

//------------------------------------------------------------------------------
void Anchor::apply()
{
	ZN_LOG_ONCE(warn, "Anchor::apply: not implemented yet");

//	Camera * guiCamera =

}

//------------------------------------------------------------------------------
void Anchor::serializeData(JsonBox::Value& o)
{
	ABehaviour::serialize(o);
	zn::serialize(o["align"], m_align);
}

//------------------------------------------------------------------------------
void Anchor::unserializeData(JsonBox::Value& o)
{
	ABehaviour::unserialize(o);
	zn::serialize(o["align"], m_align);
}

} // namespace zn



