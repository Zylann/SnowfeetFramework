#include <iostream>
#include "fm/proto/Entity.hpp"
#include "fm/proto/physics/BoxCollider.hpp"
#include "Character.hpp"
#include "Mover.hpp"

namespace demo
{

void Character::init()
{
	m_spriteDir = Direction::EAST;
	m_spriteDirLastFrame = Direction::EAST;
	r_sprite = entity().addComponent<SpriteRenderer>();

	r_mover = entity().addComponent<Mover>();

	m_movingLastFrame = false;
}

void Character::setSpriteDirection(u8 dir)
{
	m_spriteDir = dir;
}

void Character::onUpdate()
{
	bool moving = r_mover->moving();

	if(m_spriteDir != m_spriteDirLastFrame || (moving ^ m_movingLastFrame))
	{
		m_spriteDirLastFrame = m_spriteDir;
		m_movingLastFrame = moving;

		AAnimator * anim = entity().animator();

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

} // namespace demo

