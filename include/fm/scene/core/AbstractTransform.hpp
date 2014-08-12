#ifndef HEADER_ZN_ABSTRACTTRANSFORM_HPP_INCLUDED
#define HEADER_ZN_ABSTRACTTRANSFORM_HPP_INCLUDED

#include <fm/types.hpp>
#include <SFML/Graphics.hpp> // for sf::Transform...
#include <fm/scene/core/Component.hpp>

namespace zn
{

/// \brief Represent the location and disposition of an object in space.
/// Units are arbitrary. Generally, try to match physics and graphics,
/// as it involves less computations.
/// A transform can be a single couple of coordinates, or a complex local/global
/// set of matrices, so the purpose of this interface is to allow you to define
/// your own.
/// The engine works with floats, so if for instance your game uses integer units for positions,
/// you can inherit your custom location from this class, so the engine knows where to pick it.
///
/// Use:
/// Remember that a Transform is a component, so you may define it like this:
/// class MyTransform : public Component, public ITransform
///
/// \see Transform (built-in)
class ZN_API AbstractTransform : public Component
{
public:

	ZN_ABSTRACT_OBJECT(zn::AbstractTransform, zn::Component)

	// Required

	virtual sf::Vector2f position() const = 0;
	virtual void setPosition(sf::Vector2f newPos) = 0;

	// Optional

	virtual sf::Vector2f scale() const { return sf::Vector2f(1,1); }
	virtual void setScale(sf::Vector2f newScale) {}

	/// \brief Gets the rotation of the object.
	/// \return angle in degrees.
	virtual f32 rotation() const { return 0; }

	/// \brief Sets the rotation of this object.
	/// \param newRotation: angle in degrees.
	virtual void setRotation(f32 degrees) {}

	/// \brief Returns the global transformation matrix of this transform.
	/// If it has no such matrix, it will get recomputed on the fly from above
	/// properties (by the graphics system mostly).
	virtual const sf::Transform * matrix() const { return nullptr; }

	/// \brief Called when the parent of the entity owning the component
	/// has a new parent or has been unparented
	virtual void onParentChanged() {}

	// Delta transformations

	virtual void translate(sf::Vector2f motion);
	virtual void rotate(f32 degrees);
	virtual void scale(sf::Vector2f scaleFactor);

};

} // namespace zn

#endif // HEADER_ZN_ABSTRACTTRANSFORM_HPP_INCLUDED


