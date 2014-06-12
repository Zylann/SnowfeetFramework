#ifndef HEADER_ZN_BODY_HPP_INCLUDED
#define HEADER_ZN_BODY_HPP_INCLUDED

#include <fm/scene/core/Component.hpp>

namespace zn
{

// Physics
// Note: the engine doesn't supports physics yet.
// TODO Body: interface with Box2D
class ZN_API Body : public Component
{
public:

	ZN_OBJECT(zn::Body, zn::Component)

	void onAdd(Entity * e) override;
	void onDestroy() override;

	virtual void accelerate(const sf::Vector2f & acc) {}

};

} // namespace zn

#endif // HEADER_ZN_BODY_HPP_INCLUDED

