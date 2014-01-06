//  __                      __           _
// / _\_ __   _____      __/ _| ___  ___| |_
// \ \| '_ \ / _ \ \ /\ / / |_ / _ \/ _ \ __|
// _\ \ | | | (_) \ V  V /|  _|  __/  __/ |_
// \__/_| |_|\___/ \_/\_/ |_|  \___|\___|\__|
//         S P A C E     M A D N E S S
//
// This file is part of the Snowfeet Space Madness game.
// Copyright Marc Gilleron 2013
//

#ifndef HEADER_SSM_AVATAR_HPP_INCLUDED
#define HEADER_SSM_AVATAR_HPP_INCLUDED

#include "fm/proto/Component.hpp"
#include "fm/proto/graphics/SpriteRenderer.hpp"
#include "Character.hpp"

namespace demo
{

class Avatar : public Character
{
public:

	ZN_BEHAVIOUR(demo::Avatar, 10)

	void init() override;
	void onUpdate() override;

	void onCollisionEnter(const CollisionInfo & info) override;

private:

	bool m_pressedLastFrame;

};

} // namespace demo

#endif // HEADER_SSM_AVATAR_HPP_INCLUDED
