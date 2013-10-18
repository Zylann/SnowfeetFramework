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

#include <iostream>
#include "fm/game2d/Entity.hpp"
#include "fm/game2d/components/BoxCollider.hpp"
#include "Character.hpp"
#include "Mover.hpp"

namespace ssm
{

void Character::init()
{
	m_spriteDir = Direction::EAST;
	m_spriteDirLastFrame = Direction::EAST;
	r_sprite = owner().addComponent<SpriteRenderer>();

	r_mover = owner().addComponent<Mover>();

	m_movingLastFrame = false;
}

void Character::setSpriteDirection(u8 dir)
{
	m_spriteDir = dir;
}

void Character::update()
{
	// TODO update transform in renderer
	r_sprite->setPosition(owner().position());

	bool moving = r_mover->moving();

	if(m_spriteDir != m_spriteDirLastFrame || (moving ^ m_movingLastFrame))
	{
		m_spriteDirLastFrame = m_spriteDir;
		m_movingLastFrame = moving;

		AAnimator * anim = owner().animator();

		if(anim != nullptr)
		{
			std::string dirName;

			switch(m_spriteDir)
			{
			case Direction::WEST:
				dirName = ".left";
				break;

			case Direction::EAST:
				dirName = ".right";
				break;

			case Direction::NORTH:
				dirName = ".up";
				break;

			case Direction::SOUTH:
				dirName = ".down";
				break;

			default:
				break;
			}

			//std::cout << "up" << std::endl;

			if(moving)
			{
				anim->play("walk" + dirName);
			}
			else
			{
				anim->stop();
				r_sprite->setFrame("idle" + dirName);
			}
		}
	}
}

} // namespace ssm

