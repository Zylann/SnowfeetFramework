#ifndef HEADER_ZN_ACOLLIDER_HPP_INCLUDED
#define HEADER_ZN_ACOLLIDER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Component.hpp"
#include "../../config.hpp"
//#include "../CollisionInfo.hpp"

namespace zn
{

class ZN_API ACollider : public AComponent
{
public:

	void onAdd(Entity * e) override;
	void onDestroy() override;

	virtual bool collides(const sf::Vector2f & p) const = 0;
	virtual bool collides(const sf::FloatRect & r) const = 0;
	virtual sf::FloatRect bounds() const = 0;
	//virtual bool raycast(const Ray & ray, RayHit * hit=nullptr, f32 maxDistance=100f) const = 0;

	// Serialization
	virtual void serializeData(JsonBox::Value & o) override;
	virtual void unserializeData(JsonBox::Value & o) override;
	virtual void postUnserialize() override;

#ifdef ZN_DEBUG
	// Renders the boundaries or the collider
	virtual void debug_draw(sf::RenderTarget & target) const = 0;
#endif

};

} // namespace zn

#endif // HEADER_ZN_ACOLLIDER_HPP_INCLUDED
