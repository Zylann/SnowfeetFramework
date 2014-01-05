#ifndef HEADER_ZN_DIRECTION_HPP_INCLUDED
#define HEADER_ZN_DIRECTION_HPP_INCLUDED

#include <SFML/System.hpp>
#include "../types.hpp"

//#define ZN_Y_UP

namespace zn
{

// Y-down 2D 4-direction helper
namespace Direction
{

	// Contiguous direction identifiers
	enum
	{
		EAST   = 0,
		WEST   = 1,
		NORTH  = 2,
		SOUTH  = 3,
		NONE   = 4
	};

	// Bitmasks
	enum
	{
		EAST_BIT  = 1 << EAST,
		WEST_BIT  = 1 << WEST,
		NORTH_BIT = 1 << NORTH,
		SOUTH_BIT = 1 << SOUTH,
		NONE_BITS = 0,
		ANY_BITS  = EAST_BIT | WEST_BIT | NORTH_BIT | SOUTH_BIT
	};

	inline u8 opposite(u8 dir)
	{
		switch(dir)
		{
		case EAST: return WEST;
		case WEST: return EAST;
		case NORTH: return SOUTH;
		case SOUTH: return NORTH;
		default: return NONE;
		}
	}

	inline u8 left(u8 dir)
	{
		switch(dir)
		{
		case EAST: return NORTH;
		case WEST: return SOUTH;
		case NORTH: return WEST;
		case SOUTH: return EAST;
		default: return NONE;
		}
	}

	inline u8 right(u8 dir)
	{
		switch(dir)
		{
		case EAST: return SOUTH;
		case WEST: return NORTH;
		case NORTH: return EAST;
		case SOUTH: return WEST;
		default: return NONE;
		}
	}

	template <typename T>
	inline sf::Vector2<T> toVector(u8 dir)
	{
		switch(dir)
		{
		case EAST: return sf::Vector2<T>(1, 0);
		case WEST: return sf::Vector2<T>(-1, 0);
#ifdef ZN_Y_UP
		case NORTH: return sf::Vector2<T>(0, 1);
		case SOUTH: return sf::Vector2<T>(0, -1);
#else
		case NORTH: return sf::Vector2<T>(0, -1);
		case SOUTH: return sf::Vector2<T>(0, 1);
#endif
		default: return sf::Vector2<T>(0,0);
		}
	}

	template <typename T>
	inline u8 toDirectionOrtho(const sf::Vector2<T> & v)
	{
		if(v.x > 0)
			return EAST;
		if(v.x < 0)
			return WEST;
#ifdef ZN_Y_UP
		if(v.y > 0)
			return NORTH;
		if(v.y < 0)
			return SOUTH;
#else
		if(v.y > 0)
			return SOUTH;
		if(v.y < 0)
			return NORTH;
#endif
		return NONE;
	}

	template <typename T>
	inline u8 toHorizontalDirection(T x)
	{
		if(x > 0)
			return Direction::EAST;
		else if(x < 0)
			return Direction::WEST;
		else
			return Direction::NONE;
	}

	template <typename T>
	inline u8 toVerticalDirection(T y)
	{
#ifdef ZN_Y_UP
		if(y > 0)
			return Direction::NORTH;
		else if(y < 0)
			return Direction::SOUTH;
#else
		if(y > 0)
			return Direction::SOUTH;
		else if(y < 0)
			return Direction::NORTH;
#endif
		else
			return Direction::NONE;
	}

	template <typename T>
	inline u8 toDirectionApprox(const sf::Vector2<T> & v)
	{
		if(v.x > 0)
		{
			if(v.y > 0)
			{
				if(v.x > v.y)
					return EAST;
				else
#ifdef ZN_Y_UP
					return NORTH;
#else
					return SOUTH;
#endif
			}
			else if(v.y < 0)
			{
				if(v.x > -v.y)
					return EAST;
				else
#ifdef ZN_Y_UP
					return SOUTH;
#else
					return NORTH;
#endif
			}
		}
		else if(v.x < 0)
		{
			if(v.y > 0)
			{
				if(-v.x > v.y)
					return WEST;
				else
#ifdef ZN_Y_UP
					return NORTH;
#else
					return SOUTH;
#endif
			}
			else if(v.y < 0)
			{
				if(-v.x > -v.y)
					return WEST;
				else
#ifdef ZN_Y_UP
					return SOUTH;
#else
					return NORTH;
#endif
			}
		}

		return NONE;
	}

	inline std::string toString(u8 dir)
	{
		switch(dir)
		{
		case EAST: return "east";
		case WEST: return "west";
		case NORTH: return "north";
		case SOUTH: return "south";
		default: return "none";
		}
	}

};

} // namespace zn

#endif // HEADER_ZN_DIRECTION_HPP_INCLUDED


