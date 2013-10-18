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

#include "fm/game2d/components/Component.hpp"
#include "fm/game2d/components/SpriteRenderer.hpp"
#include "Character.hpp"

namespace ssm
{

class Avatar : public Character
{
public:

	ZN_BEHAVIOUR(ssm::Avatar)

	void init() override;
	void update() override;

	void onCollisionEnter(const CollisionInfo & info) override;

};

} // namespace ssm

#endif // HEADER_SSM_AVATAR_HPP_INCLUDED

