#ifndef JSON_UTILS_HPP_INCLUDED
#define JSON_UTILS_HPP_INCLUDED

#include <cassert>
#include <JsonBox.h>
#include <vector>
#include <unordered_set>
#include <fm/util/Range.hpp>

#ifdef ZN_SFML
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector3.hpp>
#endif

// Json serialization helpers for various composed types

namespace zn
{

/// \brief Utility function that loads a JSON file, prints messages and checks for a version member in the document.
/// \param document: the JSON data tree to load
/// \param filePath: path to the JSON file
/// \param checkVersion: the version to check in the file. If -1, no check will be made. If >= 0, a check will be made,
/// and an error will be generated if it doesn't matches.
/// \param openError: should the function generate an error message if the file can't be open?
/// \return false if the file could'nt be loaded or if the version didn't matched, true if success.
bool loadFromFile(JsonBox::Value & document, const std::string & filePath, s32 checkVersion=-1, bool openError=true);

inline void serialize(JsonBox::Value & o, f32 v) { o = v; }
inline void serialize(JsonBox::Value & o, s32 v) { o = v; }
inline void serialize(JsonBox::Value & o, u32 v) { o = (s32)v; }
inline void serialize(JsonBox::Value & o, u8 v) { o = (s32)v; }
inline void serialize(JsonBox::Value & o, const std::string & s) { o = s; }

inline void unserialize(const JsonBox::Value & o, f32 & v) { v = o.getDouble(); }
inline void unserialize(const JsonBox::Value & o, s32 & v) { v = o.getInt(); }
inline void unserialize(const JsonBox::Value & o, u32 & v) { v = o.getInt(); }
inline void unserialize(const JsonBox::Value & o, u8 & v) { v = o.getInt(); }
inline void unserialize(const JsonBox::Value & o, std::string & s) { s = o.getString(); }

template <typename T>
inline void serialize(JsonBox::Value & o, const Range<T> & range)
{
	o[(size_t)0] = range.min();
	o[1] = range.max();
}

inline void unserialize(JsonBox::Value & o, Range<f32> & range)
{
	range.set(o[(size_t)0].getDouble(), o[1].getDouble());
}

inline void unserialize(JsonBox::Value & o, Range<s32> & range)
{
	range.set(o[(size_t)0].getInt(), o[1].getInt());
}

template <typename List_T>
void serialize(JsonBox::Value & o, const List_T & v)
{
	JsonBox::Array a;
	a.resize(v.size());
	u32 i = 0;
	for(auto it = v.cbegin(); it != v.cend(); ++it)
	{
		a[i] = *it;
		++it;
		++i;
	}
	o = a;
}

template <typename T>
void unserialize(const JsonBox::Value & o, std::vector<T> & v)
{
	const JsonBox::Array & a = o.getArray();
	v.resize(a.size());
	for(u32 i = 0; i < a.size(); ++i)
	{
		const JsonBox::Value & jv = a[i];
		unserialize(jv, v[i]);
	}
}

template <typename T>
void unserialize(const JsonBox::Value & o, std::unordered_set<T> & v)
{
	const JsonBox::Array & a = o.getArray();
	for(u32 i = 0; i < a.size(); ++i)
	{
		T elem;
		unserialize(a[i], elem);
		v.insert(elem);
	}
}

#ifdef ZN_SFML

template <typename T>
inline void serialize(JsonBox::Value & o, const sf::Vector2<T> & vec)
{
	o[(size_t)0] = vec.x;
	o[1] = vec.y;
}

template <typename T>
inline void serialize(JsonBox::Value & o, const sf::Vector3<T> & vec)
{
	o[(size_t)0] = vec.x;
	o[1] = vec.y;
	o[2] = vec.z;
}

inline void unserialize(JsonBox::Value & o, sf::Vector2f & vec)
{
	vec.x = o[(size_t)0].getDouble();
	vec.y = o[1].getDouble();
}

inline void unserialize(JsonBox::Value & o, sf::Vector3f & vec)
{
	vec.x = o[(size_t)0].getDouble();
	vec.y = o[1].getDouble();
	vec.z = o[2].getDouble();
}

inline void unserialize(JsonBox::Value & o, sf::Vector3i & vec)
{
	vec.x = o[(size_t)0].getInt();
	vec.y = o[1].getInt();
	vec.z = o[2].getInt();
}

inline void unserialize(JsonBox::Value & o, sf::Vector2i & vec)
{
	vec.x = o[(size_t)0].getInt();
	vec.y = o[1].getInt();
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
	o[(size_t)0] = color.r;
	o[1] = color.g;
	o[2] = color.b;
	o[3] = color.a;
}

inline void unserialize(JsonBox::Value & o, sf::Color & color)
{
	color.r = o[(size_t)0].getInt();
	color.g = o[1].getInt();
	color.b = o[2].getInt();
	color.a = o[3].getInt();
}

inline void serialize(JsonBox::Value & o, const sf::Transform & t)
{
	const float * v = t.getMatrix();

	JsonBox::Array a;
	a.resize(16);

	for(u32 i = 0; i < 16; ++i)
	{
		a[i] = v[i];
	}

	o = a;
}

inline void unserialize(const JsonBox::Value & o, sf::Transform & t)
{
	assert(o.isArray());

	const JsonBox::Array & a = o.getArray();

	t = sf::Transform(
		a[0].getDouble(), a[4].getDouble(), a[12].getDouble(),
		a[1].getDouble(), a[5].getDouble(), a[13].getDouble(),
		a[3].getDouble(), a[7].getDouble(), a[15].getDouble()
	);
}

#endif

}

#endif // JSON_UTILS_HPP_INCLUDED


