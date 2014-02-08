#ifndef HEADER_ZN_BOXCOLLIDER_HPP_INCLUDED
#define HEADER_ZN_BOXCOLLIDER_HPP_INCLUDED

#include "Collider.hpp"

namespace zn
{

class ZN_API BoxCollider : public ACollider
{
public:

	ZN_COMPONENT(zn::BoxCollider, CG_COLLIDER, 0)

	bool collides(const sf::Vector2f & p) const override;
	bool collides(const sf::FloatRect & r0) const override;
	sf::FloatRect bounds() const override;

	inline const sf::FloatRect & rect() const { return m_rect; }
	void setRect(const sf::FloatRect & r);

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

#ifdef ZN_DEBUG
	void debug_draw(sf::RenderTarget & target) const override;
#endif

private:

	sf::FloatRect m_rect;

};

} // namespace zn

#endif // HEADER_ZN_BOXCOLLIDER_HPP_INCLUDED

