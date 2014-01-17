#include <fm/proto/Entity.hpp>
#include <fm/proto/physics/BoxCollider.hpp>
#include <fm/sfml/sfml2_utils.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

//------------------------------------------------------------------------------
void BoxCollider::setRect(const sf::FloatRect& r)
{
	m_rect = r;
}

//------------------------------------------------------------------------------
bool BoxCollider::collides(const sf::Vector2f& p) const
{
	return m_rect.contains(p - entity().transform.position());
}

//------------------------------------------------------------------------------
bool BoxCollider::collides(const sf::FloatRect & r0) const
{
	sf::FloatRect r(r0);
	r.left -= entity().transform.position().x;
	r.top -= entity().transform.position().y;
	return intersects(m_rect, r);
}

//------------------------------------------------------------------------------
sf::FloatRect BoxCollider::bounds() const
{
	return m_rect;
}

//------------------------------------------------------------------------------
void BoxCollider::serializeData(JsonBox::Value & o)
{
	ACollider::serializeData(o);
	zn::serialize(o["rect"], m_rect);
}

//------------------------------------------------------------------------------
void BoxCollider::unserializeData(JsonBox::Value & o)
{
	ACollider::unserializeData(o);
	zn::unserialize(o["rect"], m_rect);
}

//------------------------------------------------------------------------------
void BoxCollider::postUnserialize()
{
	ACollider::postUnserialize();
}

//------------------------------------------------------------------------------
#ifdef ZN_DEBUG
void BoxCollider::debug_draw(sf::RenderTarget & target) const
{
	static sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(m_rect.width, m_rect.height));
	rect.setPosition(sf::Vector2f(m_rect.left, m_rect.top) + entity().transform.position());
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1);
	rect.setFillColor(sf::Color::Transparent);
	target.draw(rect);
}
#endif

} // namespace zn



