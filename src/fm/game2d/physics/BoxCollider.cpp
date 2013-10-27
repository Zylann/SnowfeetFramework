#include "../Entity.hpp"
#include "../../sfml/sfml2_utils.hpp"
#include "BoxCollider.hpp"

namespace zn
{

void BoxCollider::setRect(const sf::FloatRect& r)
{
	_rect = r;
}

bool BoxCollider::collides(const sf::Vector2f& p) const
{
	return _rect.contains(p - entity().position());
}

bool BoxCollider::collides(const sf::FloatRect & r0) const
{
	sf::FloatRect r(r0);
	r.left -= entity().position().x;
	r.top -= entity().position().y;
	return intersects(_rect, r);
}

sf::FloatRect BoxCollider::bounds() const
{
	return _rect;
}

#ifdef ZN_DEBUG
void BoxCollider::debug_draw(sf::RenderTarget & target) const
{
	static sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(_rect.width, _rect.height));
	rect.setPosition(sf::Vector2f(_rect.left, _rect.top) + entity().position());
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1);
	rect.setFillColor(sf::Color::Transparent);
	target.draw(rect);
}
#endif

} // namespace zn

