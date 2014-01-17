#ifndef JSON_UTILS_HPP_INCLUDED
#define JSON_UTILS_HPP_INCLUDED

#include "JsonBox.h"
#include <fm/util/Range.hpp>

#ifdef ZN_SFML
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#endif

// Json serialization helpers for various composed types

namespace zn
{

template <typename T>
inline void serialize(JsonBox::Value & o, const Range<T> & range)
{
	o["min"] = range.min();
	o["max"] = range.max();
}

inline void unserialize(JsonBox::Value & o, Range<f32> & range)
{
	range.set(o["min"].getDouble(), o["max"].getDouble());
}

inline void unserialize(JsonBox::Value & o, Range<s32> & range)
{
	range.set(o["min"].getInt(), o["max"].getInt());
}

#ifdef ZN_SFML

template <typename T>
inline void serialize(JsonBox::Value & o, const sf::Vector2<T> & vec)
{
	o["x"] = vec.x;
	o["y"] = vec.y;
}

inline void unserialize(JsonBox::Value & o, sf::Vector2f & vec)
{
	vec.x = o["x"].getDouble();
	vec.y = o["y"].getDouble();
}

inline void unserialize(JsonBox::Value & o, sf::Vector2i & vec)
{
	vec.x = o["x"].getInt();
	vec.y = o["y"].getInt();
}

template <typename T>
inline void serialize(JsonBox::Value & o, const sf::Rect<T> & rect)
{
	o["x"] = rect.left;
	o["y"] = rect.top;
	o["w"] = rect.width;
	o["h"] = rect.height;
}

inline void unserialize(JsonBox::Value & o, sf::FloatRect & rect)
{
	rect.left   = o["x"].getDouble();
	rect.top    = o["y"].getDouble();
	rect.width  = o["w"].getDouble();
	rect.height = o["h"].getDouble();
}

inline void unserialize(JsonBox::Value & o, sf::IntRect & rect)
{
	rect.left   = o["x"].getInt();
	rect.top    = o["y"].getInt();
	rect.width  = o["w"].getInt();
	rect.height = o["h"].getInt();
}

inline void serialize(JsonBox::Value & o, const sf::Color & color)
{
	o["r"] = color.r;
	o["g"] = color.g;
	o["b"] = color.b;
	o["a"] = color.a;
}

inline void unserialize(JsonBox::Value & o, sf::Color & color)
{
	color.r = o["r"].getInt();
	color.g = o["g"].getInt();
	color.b = o["b"].getInt();
	color.a = o["a"].getInt();
}

#endif

}

#endif // JSON_UTILS_HPP_INCLUDED


