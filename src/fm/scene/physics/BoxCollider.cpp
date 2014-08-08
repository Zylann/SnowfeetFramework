#include <fm/scene/core/Entity.hpp>
#include <fm/scene/physics/BoxCollider.hpp>
#include <fm/scene/graphics/Renderer.hpp>
#include <fm/sfml/sfml2_utils.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

ZN_OBJECT_IMPL(zn::BoxCollider)

//------------------------------------------------------------------------------
void BoxCollider::onCreate()
{
	Collider::onCreate();

	// Match the renderer bounds if any to initialize the default hitbox
	Renderer * r = entity().renderer();
	if(r != nullptr)
	{
		m_rect = r->localBounds();
	}
}

//------------------------------------------------------------------------------
void BoxCollider::setRect(const sf::FloatRect& r)
{
	m_rect = r;
}

//------------------------------------------------------------------------------
bool BoxCollider::collides(const sf::Vector2f& p) const
{
	if(entity().transform())
	{
		return m_rect.contains(p - entity().transform()->position());
	}
	else
	{
		return m_rect.contains(p);
	}
}

//------------------------------------------------------------------------------
bool BoxCollider::collides(const sf::FloatRect & r0) const
{
	sf::FloatRect r(r0);

	if(entity().transform())
	{
		sf::Vector2f pos = entity().transform()->position();
		r.left -= pos.x;
		r.top -= pos.y;
	}

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
	Collider::serializeData(o);
	zn::serialize(o["rect"], m_rect);
}

//------------------------------------------------------------------------------
void BoxCollider::unserializeData(JsonBox::Value & o)
{
	Collider::unserializeData(o);
	zn::unserialize(o["rect"], m_rect);
}

//------------------------------------------------------------------------------
void BoxCollider::postUnserialize()
{
	Collider::postUnserialize();
}

//------------------------------------------------------------------------------
#ifdef ZN_DEBUG
void BoxCollider::debug_draw(sf::RenderTarget & target) const
{
	static sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(m_rect.width, m_rect.height));

	if(entity().transform())
	{
		rect.setPosition(sf::Vector2f(m_rect.left, m_rect.top) + entity().transform()->position());
	}

	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(math::min(m_rect.width,m_rect.height) * 0.02);
	rect.setFillColor(sf::Color::Transparent);
	target.draw(rect);
}
#endif

} // namespace zn



