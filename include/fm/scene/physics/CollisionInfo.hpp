/*
CollisionInfo.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_COLLISIONINFO_HPP_INCLUDED
#define HEADER_ZN_COLLISIONINFO_HPP_INCLUDED

//#include "Direction.hpp"
#include <SFML/System.hpp>

namespace zn
{

class ZN_API Collider;

struct CollisionInfo
{
	Collider * collider;
	sf::Vector2f motion;

	CollisionInfo(Collider * c, const sf::Vector2f & pmotion) :
		collider(c),
		motion(pmotion)
	{}

	CollisionInfo(Collider * c) :
		collider(c)
	{}

};

} // namespace zn

#endif // HEADER_ZN_COLLISIONINFO_HPP_INCLUDED


