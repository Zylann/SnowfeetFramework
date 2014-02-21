#ifndef HEADER_DEMO_MOVER_HPP_INCLUDED
#define HEADER_DEMO_MOVER_HPP_INCLUDED

#include <fm/proto/behaviour.hpp>

#include "common.hpp"

namespace demo
{

// The engine doesn't supports physics yet.
// This component adds simple movement to an entity, with AABB physics.
// Does not work with non-AABB colliders.
class Mover : public ABehaviour
{
public:

	ZN_BEHAVIOUR(demo::Mover, 10)

	Mover();

	void init() override;
	void onUpdate() override;

	void setMaxSpeed(f32 speed);
	void accelerate(const sf::Vector2f & acc);
	void accelerate(f32 amount, const sf::Vector2i & dir);
	void setVelocity(const sf::Vector2f & velocity);

	inline const sf::Vector2f & velocity() const { return m_velocity; }
	bool moving() const;

private:

	sf::Vector2f m_acceleration;
	sf::Vector2f m_velocity;
	f32 m_maxSpeed;

};

} // namespace demo

#endif // HEADER_DEMO_MOVER_HPP_INCLUDED



