#ifndef HEADER_ZN_BODY_HPP_INCLUDED
#define HEADER_ZN_BODY_HPP_INCLUDED

#include "Component.hpp"

namespace zn
{

// Physics
// Note: the engine doesn't supports physics yet.
// TODO Body: interface with Box2D
class Body : public AComponent
{
public:

	ZN_COMPONENT(zn::Body, CG_BODY)

	void onAdd(Entity * e) override;
	void onDestroy() override;

	virtual void accelerate(const sf::Vector2f & acc) = 0;

};

} // namespace zn

#endif // HEADER_ZN_BODY_HPP_INCLUDED

