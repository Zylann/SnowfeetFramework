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

#include "Mover.hpp"
#include "fm/sfml/sfml2_utils.hpp"
#include "fm/proto/Scene.hpp"
#include "fm/proto/physics/Collider.hpp"

namespace demo
{

Mover::Mover() : ABehaviour(),
	m_maxSpeed(4)
{
}

//------------------------------------------------------------------------------
bool Mover::moving() const
{
	return !isZero(m_velocity)
		|| !isZero(m_acceleration);
}

//------------------------------------------------------------------------------
void Mover::setMaxSpeed(f32 speed)
{
	m_maxSpeed = speed < 0 ? 0 : speed;
}

//------------------------------------------------------------------------------
void Mover::init()
{
}

//------------------------------------------------------------------------------
void Mover::accelerate(const sf::Vector2f & acc)
{
	m_acceleration += acc;
}

//------------------------------------------------------------------------------
void Mover::accelerate(f32 amount, const sf::Vector2i & dir)
{
	m_acceleration += amount * sf::Vector2f(dir.x, dir.y);
}

//------------------------------------------------------------------------------
void  Mover::setVelocity(const sf::Vector2f & velocity)
{
	m_velocity = velocity;
}

//------------------------------------------------------------------------------
void Mover::onUpdate()
{
	if(isZero(m_acceleration))
	{
		// Braking
		m_velocity.x = 0;
		m_velocity.y = 0;
	}
	else
	{
		// Acceleration
		m_velocity += m_acceleration;
		m_acceleration.x = 0;
		m_acceleration.y = 0;
	}

	// Speed limitation
	f32 speed = zn::length(m_velocity);
	if(speed > m_maxSpeed && !math::isZero(speed))
	{
		m_velocity /= speed;
		speed = m_maxSpeed;
		m_velocity *= speed;
	}

	if(speed > 0.01f)
	{
		sf::Vector2f motion = m_velocity;
		const ACollider * collider = entity().collider();
		sf::FloatRect rect = collider->bounds();
		sf::Vector2f pos = entity().position();
		sf::Vector2f boxOffset(rect.left, rect.top);
		rect.left += pos.x;
		rect.top += pos.y;

		const f32 fraction = 1.f;
		sf::Vector2f u = fraction * motion;

		ACollider * otherCollider;

		for(u8 i=0; i<32 && !isZero(motion); ++i)
		{
			if(!math::isZero(motion.x))
			{
				rect.left += u.x;
				otherCollider = entity().scene().colliderAt(rect, collider);
				if(otherCollider != nullptr)
				{
					// Horizontal collision
					entity().onCollisionEnter(CollisionInfo(otherCollider));
					rect.left -= u.x;
					motion.x = 0;
					u.x = 0;
				}
				else
				{
					motion.x -= u.x;
					if(math::isZero(motion.x))
					{
						motion.x = 0;
						u.x = 0;
					}
				}
			}

			if(!math::isZero(motion.y))
			{
				rect.top += u.y;
				otherCollider = entity().scene().colliderAt(rect, collider);
				if(otherCollider != nullptr)
				{
					// Vertical collision
					entity().onCollisionEnter(CollisionInfo(otherCollider));
					rect.top -= u.y;
					motion.y = 0;
					u.y = 0;
				}
				else
				{
					motion.y -= u.y;
					if(math::isZero(motion.y))
					{
						motion.y = 0;
						u.y = 0;
					}
				}
			}
		}

		pos.x = rect.left - boxOffset.x;
		pos.y = rect.top - boxOffset.y;
		entity().setPosition(pos);
	}
}

} // namespace demo

