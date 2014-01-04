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

#ifndef HEADER_SSM_CAT_HPP_INCLUDED
#define HEADER_SSMCAT_HPP_INCLUDED

#include "Character.hpp"

namespace ssm
{

class Cat : public Character
{
public:

	ZN_BEHAVIOUR(ssm::Cat, 10)

	void init() override;
	void onUpdate() override;

private:

	f32 m_timeBeforeAction;
	u8 m_motorDir;

};

} // namespace ssm

#endif // HEADER_SSMCAT_HPP_INCLUDED

