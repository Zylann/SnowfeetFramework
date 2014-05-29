#include <iostream>

#include <fm/scene/physics.hpp>

#include "Character.hpp"
#include "Mover.hpp"

namespace demo
{

void Character::onCreate()
{
	m_spriteDir = Direction::RIGHT;
	m_spriteDirLastFrame = Direction::RIGHT;
	r_sprite = entity().addComponent<SpriteRenderer>();
	r_sprite->setScale(1.f / DEMO_TS);

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
			case Direction::LEFT:
				dirName = ".left";
				break;

			case Direction::RIGHT:
				dirName = ".right";
				break;

			case Direction::UP:
				dirName = ".up";
				break;

			case Direction::DOWN:
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

