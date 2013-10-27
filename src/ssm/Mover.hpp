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

#ifndef HEADER_SSM_MOVER_HPP_INCLUDED
#define HEADER_SSM_MOVER_HPP_INCLUDED

#include "fm/game2d/behaviour/Behaviour.hpp"
#include "common.hpp"

namespace ssm
{

// The engine doesn't supports physics yet.
// This component adds simple movement to an entity, with AABB physics.
// Does not work with non-AABB colliders.
class Mover : public ABehaviour
{
public:

	ZN_BEHAVIOUR(ssm::Mover)

	Mover();

	void init() override;
	void update() override;

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

} // namespace ssm

#endif // HEADER_SSM_MOVER_HPP_INCLUDED



