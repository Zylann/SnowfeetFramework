#include <fm/sfml/sfml2_utils.hpp>
#include <fm/scene/physics.hpp>

#include "Mover.hpp"

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
void Mover::onCreate()
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
	f32 dt = entity().scene().deltaTime().asSeconds();

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
		sf::Vector2f motion = m_velocity * dt;
		const ACollider * collider = entity().collider();
		sf::FloatRect rect = collider->bounds();
		sf::Vector2f pos = entity().transform.position();
		sf::Vector2f boxOffset(rect.left, rect.top);
		rect.left += pos.x;
		rect.top += pos.y;

		//std::cout << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << std::endl;

		const s32 fractionCount = 4;
		const f32 fraction = 1.f / static_cast<f32>(fractionCount);
		sf::Vector2f u = fraction * motion;

		ACollider * otherCollider;

		// While motion is not zero
		for(u8 i=0; i<fractionCount && !isZero(motion); ++i)
		{
			// If motion remains on X
			if(!math::isZero(motion.x))
			{
				// Advance a bit on X
				rect.left += u.x;
				otherCollider = entity().scene().physics.colliderAt(rect, collider);
				// If we collide horizontally
				if(otherCollider != nullptr)
				{
					// Horizontal collision event
					entity().onCollisionEnter(CollisionInfo(otherCollider));
					// Go back
					rect.left -= u.x;
					// Stop motion on X
					motion.x = 0;
					u.x = 0;
				}
				else
				{
					// Remove the part of motion we just moved
					motion.x -= u.x;
					// If there is no more to advance on X
					if(math::isZero(motion.x))
					{
						// Finished to advance on X
						motion.x = 0;
						u.x = 0;
					}
				}
			}

			// Same on Y
			if(!math::isZero(motion.y))
			{
				rect.top += u.y;
				otherCollider = entity().scene().physics.colliderAt(rect, collider);
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
		entity().transform.setPosition(pos);
	}
}

} // namespace demo

