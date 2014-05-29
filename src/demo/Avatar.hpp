#ifndef HEADER_DEMO_AVATAR_HPP_INCLUDED
#define HEADER_DEMO_AVATAR_HPP_INCLUDED

#include "Character.hpp"

namespace demo
{

class Avatar : public Character
{
public:

	ZN_COMPONENT(demo::Avatar, demo::Character, 10)

	void onCreate() override;
	void onUpdate() override;

	void onCollisionEnter(const CollisionInfo & info) override;

private:

	bool m_pressedLastFrame;

};

} // namespace demo

#endif // HEADER_SSM_AVATAR_HPP_INCLUDED

