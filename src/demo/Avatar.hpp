#ifndef HEADER_DEMO_AVATAR_HPP_INCLUDED
#define HEADER_DEMO_AVATAR_HPP_INCLUDED

#include "Character.hpp"

namespace demo
{

class Avatar : public Character
{
public:

	ZN_OBJECT(demo::Avatar, demo::Character)

	s32 getUpdateOrder() const override { return 10; }

	void onCreate() override;
	void onUpdate() override;

	void onCollisionEnter(const CollisionInfo & info) override;

private:

	bool m_pressedLastFrame;

};

} // namespace demo

#endif // HEADER_SSM_AVATAR_HPP_INCLUDED

