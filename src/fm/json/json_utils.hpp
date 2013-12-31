#ifndef JSON_UTILS_HPP_INCLUDED
#define JSON_UTILS_HPP_INCLUDED

#include <SFML/Graphics/Rect.hpp>
#include "JsonBox.h"
#include "../util/Range.hpp"

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
void serialize(JsonBox::Value & o, const sf::Rect<T> rect)
{
	o["x"] = rect.left;
	o["y"] = rect.top;
	o["w"] = rect.width;
	o["h"] = rect.height;
}

inline void unserialize(JsonBox::Value & o, sf::FloatRect rect)
{
	rect.left   = o["x"].getDouble();
	rect.top    = o["y"].getDouble();
	rect.width  = o["w"].getDouble();
	rect.height = o["h"].getDouble();
}

inline void unserialize(JsonBox::Value & o, sf::IntRect rect)
{
	rect.left   = o["x"].getInt();
	rect.top    = o["y"].getInt();
	rect.width  = o["w"].getInt();
	rect.height = o["h"].getInt();
}

// TODO serialization util sf::Transformable

#endif

}

#endif // JSON_UTILS_HPP_INCLUDED


