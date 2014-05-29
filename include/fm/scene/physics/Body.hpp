#ifndef HEADER_ZN_BODY_HPP_INCLUDED
#define HEADER_ZN_BODY_HPP_INCLUDED

#include <fm/scene/core/Component.hpp>

namespace zn
{

// Physics
// Note: the engine doesn't supports physics yet.
// TODO Body: interface with Box2D
class ZN_API Body : public AComponent
{
public:

	ZN_COMPONENT(zn::Body, CG_BODY, 0)

	void onAdd(Entity * e) override;
	void onDestroy() override;

	virtual void accelerate(const sf::Vector2f & acc) {}

};

} // namespace zn

#endif // HEADER_ZN_BODY_HPP_INCLUDED

