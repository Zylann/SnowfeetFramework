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

#ifndef HEADER_SSM_CHARACTER_HPP_INCLUDED
#define HEADER_SSM_CHARACTER_HPP_INCLUDED

#include "fm/proto/graphics/SpriteRenderer.hpp"
#include "fm/proto/Direction.hpp"
#include "common.hpp"
#include "Mover.hpp"

namespace ssm
{

// Base component for characters that can move and animate
class Character : public zn::ABehaviour
{
public:

//	ZN_BEHAVIOUR(ssm::Character)

	Character() : ABehaviour(),
		r_sprite(nullptr),
		r_mover(nullptr),
		m_spriteDir(Direction::SOUTH)
	{}

	virtual void init() override;

	inline u8 spriteDirection() const { return m_spriteDir; }
	void setSpriteDirection(u8 dir);

protected:

	SpriteRenderer * r_sprite;
	Mover *          r_mover;

	void onUpdate() override;

private:

	u8    m_spriteDir;
	u8    m_spriteDirLastFrame;
	bool  m_movingLastFrame;

};

} // namespace ssm

#endif // HEADER_SSM_CHARACTER_HPP_INCLUDED


