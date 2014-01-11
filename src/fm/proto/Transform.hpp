#ifndef HEADER_ZN_PROTO_TRANSFORM_HPP_INCLUDED
#define HEADER_ZN_PROTO_TRANSFORM_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "../json/json_utils.hpp"

namespace zn
{

class Entity;

// Represents the spatial transformation applied to an entity,
// as well as its place in the hierarchy (children, parent).
// Note that only entities are allowed to create such objects.
class Transform
{
public:

	typedef std::vector<Transform*> container;
	typedef container::iterator iterator;
	typedef container::const_iterator const_iterator;

	Transform(Entity & e);

	Entity & entity() const { return r_entity; }

	//-------------------------
	// Transformation
	//-------------------------

	sf::Vector2f localPosition() const;
	sf::Vector2f position() const;
	sf::Vector2f localScale() const;
	sf::Vector2f scale() const;
	f32 localRotation() const; // in degrees
	f32 rotation() const; // in degrees

	void setLocalPosition(const sf::Vector2f & newPosition);
	void setPosition(sf::Vector2f newPosition);
	void setLocalScale(const sf::Vector2f & newScale);
	void setScale(sf::Vector2f newScale);
	void setLocalRotation(f32 newRotation); // in degrees
	void setRotation(f32 newRotation); // in degrees

	void translate(sf::Vector2f motion);
	void rotate(f32 degrees);
	void scale(sf::Vector2f scaleFactor);

	const sf::Transform & localMatrix() const;
	const sf::Transform & matrix() const;

	//-------------------------
	// Hierarchy
	//-------------------------

	Transform * parent() const;
	void setParent(Transform * parent);
	inline void setParent(Transform & parent) { setParent(&parent); }
	inline u32 childCount() const { return m_children.size(); }
	Transform * child(u32 index) const;

	inline iterator begin()               { return m_children.begin(); }
	inline iterator end()                 { return m_children.end(); }
	inline const_iterator cbegin() const  { return m_children.cbegin(); }
	inline const_iterator cend() const    { return m_children.cend(); }

	//-------------------------
	// Serialization
	//-------------------------

	void serialize(JsonBox::Value & o);
	void unserialize(JsonBox::Value & o);
	void postUnserialize(JsonBox::Value & o);

private:

	void notifyChildrenForParentChange();

	// Called by setParent(transform) from another Transform
	void onAddChild(Transform * child);
	// Called by setParent(nullptr) from another Transform
	void onRemoveChild(Transform * child);

	Entity & r_entity;
	Transform * r_parent;
	container m_children;

	// Local
	sf::Vector2f m_position;
	sf::Vector2f m_scale;
	f32 m_rotation;

	mutable sf::Transform m_localMatrix;
	mutable sf::Transform m_globalMatrix;

	mutable bool m_localMatrixNeedUpdate;
	mutable bool m_globalMatrixNeedUpdate;

};

} // namespace zn

#endif // HEADER_ZN_PROTO_TRANSFORM_HPP_INCLUDED


