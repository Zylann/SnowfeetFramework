#ifndef HEADER_ZN_SFML2_UTILS_HPP_INCLUDED
#define HEADER_ZN_SFML2_UTILS_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "../util/math.hpp"

// Utility functions that extend SFML functionnality,
// mainly for clarity and shorter code

namespace zn
{

template <typename T>
inline float distance(const sf::Vector2<T> & a, const sf::Vector2<T> & b)
{
	return sqrt(math::sq(a.x-b.x) + math::sq(a.y-b.y));
}

template <typename T>
inline float length(const sf::Vector2<T> & a)
{
	return sqrt(math::sq(a.x) + math::sq(a.y));
}

template <typename T>
inline void normalize(sf::Vector2<T> & a)
{
	T d = length(a);
	a.x /= d;
	a.y /= d;
}

inline bool checkTextureRect(const sf::Texture & texture, const sf::IntRect rect)
{
	return rect.left >= 0
		&& rect.top >= 0
		&& rect.width > 0
		&& rect.height > 0
		&& rect.left < static_cast<s32>(texture.getSize().x)
		&& rect.top < static_cast<s32>(texture.getSize().y)
		&& rect.left + rect.width <= static_cast<s32>(texture.getSize().x)
		&& rect.top + rect.height <= static_cast<s32>(texture.getSize().y);
}

template <typename T>
inline T minX(const sf::Rect<T> & r) { return r.left; }

template <typename T>
inline T minY(const sf::Rect<T> & r) { return r.top; }

template <typename T>
inline T maxX(const sf::Rect<T> & r) { return r.left+r.width-1; }

template <typename T>
inline T maxY(const sf::Rect<T> & r) { return r.top+r.height-1; }

// JUST tests if two rectangles are intersecting.
// This replaces the sf::Rect::intersects method, which does a few things
// that are not necessarily wanted, like intersection rect computation.
template <typename T>
inline bool intersects(const sf::Rect<T> & a, const sf::Rect<T> & b)
{
	return a.left+a.width-1 >= b.left
		&& a.top+a.height-1 >= b.top
		&& b.left+b.width-1 >= a.left
		&& b.top+b.height-1 >= a.top;
}

inline bool isZero(const sf::Vector2f & v)
{
	return math::isZero(v.x) && math::isZero(v.y);
}

};

#endif // HEADER_ZN_SFML2_UTILS_HPP_INCLUDED


