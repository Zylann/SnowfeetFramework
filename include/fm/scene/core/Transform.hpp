#ifndef HEADER_ZN_PROTO_TRANSFORM_HPP_INCLUDED
#define HEADER_ZN_PROTO_TRANSFORM_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <fm/json/json_utils.hpp>

#include <fm/scene/core/AbstractTransform.hpp>

namespace zn
{

class Entity;

/// \brief Built-in float transform containing a local and global set of
/// position, scale and rotations.
/// It also contains entity hierarchy information.
/// \note In the future, the hierarchy might move to Entity directly.
class ZN_API Transform : public AbstractTransform
{
public:

	ZN_OBJECT(zn::Transform, zn::AbstractTransform)

//	typedef std::vector<Transform*> container;
//	typedef container::iterator iterator;
//	typedef container::const_iterator const_iterator;

	Transform();

	//-------------------------
	// Transformation
	//-------------------------

	// Local transform accessors

	sf::Vector2f localPosition() const;
	sf::Vector2f localScale() const;
	f32 localRotation() const; // in degrees

	void setLocalPosition(const sf::Vector2f & newPosition);
	void setLocalScale(const sf::Vector2f & newScale);
	void setLocalRotation(f32 newRotation); // in degrees

	// Global transform accessors
	// Important note: these are a lot slower than local ones,
	// because the hierarchy transforms gets recomputed each time.
	// Use them carefully (get them in temporary variables for instance).

	sf::Vector2f position() const override;
	sf::Vector2f scale() const override;
	f32 rotation() const override;

	void setPosition(sf::Vector2f newPosition) override;
	void setScale(sf::Vector2f newScale) override;
	void setRotation(f32 newRotation) override; // in degrees

	void translate(sf::Vector2f motion) override;
	void rotate(f32 degrees) override;
	void scale(sf::Vector2f scaleFactor) override;

	// Matrices

	const sf::Transform & localMatrix() const;
	const sf::Transform * matrix() const override;

	//-------------------------
	// Hierarchy
	//-------------------------

	void onParentChanged() override;

	// Get parent transform. Returns null of the parent entity has no transform.
	AbstractTransform * parent() const;

	// Sets the parent transform. Also adds the transform to the specified parent's children list.
	// Specifying a nullptr parent means unparenting (set back to root).
//	void setParent(Transform * parent);

	// Unparents every child under this transform.
//	void unparentChildren();

	// Reference version of setParent(Transform*) for convenience
//	inline void setParent(Transform & parent) { setParent(&parent); }

	// Gets the number of children parented to this transform
//	inline u32 childCount() const { return m_children.size(); }

	// Gets a child at the given index (must be valid).
//	Transform * child(u32 index) const;

	// Iterators to children

//	inline iterator begin()               { return m_children.begin(); }
//	inline iterator end()                 { return m_children.end(); }
//	inline const_iterator cbegin() const  { return m_children.cbegin(); }
//	inline const_iterator cend() const    { return m_children.cend(); }

	//-------------------------
	// Serialization
	//-------------------------

	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	//void postUnserialize(JsonBox::Value & o) override;

private:

	// Tells children to update their global matrix when needed
	void notifyChildrenForParentChange();

	// Called by setParent(transform) from another Transform
//	void onAddChild(Transform * child);

	// Called by setParent(nullptr) from another Transform
//	void onRemoveChild(Transform * child);

//	Transform * r_parent;
//	container m_children;

	// Local decomposed transformation

	sf::Vector2f m_position;
	sf::Vector2f m_scale;
	f32 m_rotation;

	// Matrices

	mutable sf::Transform m_localMatrix;
	mutable sf::Transform m_globalMatrix;

	mutable bool m_localMatrixNeedUpdate;
	mutable bool m_globalMatrixNeedUpdate;

};

} // namespace zn

#endif // HEADER_ZN_PROTO_TRANSFORM_HPP_INCLUDED


