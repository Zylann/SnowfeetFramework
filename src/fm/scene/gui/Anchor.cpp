#include <fm/scene/gui/Anchor.hpp>
#include <fm/util/math.hpp>
#include <fm/util/macros.hpp>
#include <fm/util/Log.hpp>
#include <fm/json/json_utils.hpp>

namespace zn
{

//------------------------------------------------------------------------------
bool Anchor::onEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		apply();
	}
	return false;
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
	Behaviour::serializeData(o);
	zn::serialize(o["align"], m_align);
}

//------------------------------------------------------------------------------
void Anchor::unserializeData(JsonBox::Value& o)
{
	Behaviour::unserializeData(o);
	zn::serialize(o["align"], m_align);
}

} // namespace zn



