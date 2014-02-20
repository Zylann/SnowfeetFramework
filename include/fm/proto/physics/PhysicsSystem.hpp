#ifndef HEADER_ZN_PHYSICSSYSTEM_HPP_INCLUDED
#define HEADER_ZN_PHYSICSSYSTEM_HPP_INCLUDED

#include <fm/proto/physics/Collider.hpp>
#include <fm/proto/physics/Body.hpp>
#include <fm/proto/core/ComponentList.hpp>

namespace zn
{

/// \brief Physics manager.
/// \warning It's a _really_ simple, placeholder implemenation.
/// A way more complex physics engine will be integrated in the future (might be Box2D),
/// wrapped into this class.
class PhysicsSystem
{
public:

	/// \brief this function is called when a new collider has been created.
	/// Do not call it yourself, it's done automatically by all colliders.
	/// \param collider: newly created collider
	void registerCollider(ACollider * collider);

	/// \brief this function is called just before a collider to be destroyed.
	/// Do not call it yourself, it's done automatically by all colliders.
	/// After that call, the system won't be aware of this collider.
	/// \param collider: the collider that will be destroyed
	void unregisterCollider(ACollider * collider);

	/// \brief this function is called when a new rigidbody has been created.
	/// Do not call it yourself, it's done automatically by all rigidbodies.
	/// \param body: newly created rigidbody
	void registerBody(Body * body);

	/// \brief this function is called just before a rigidbody to be destroyed.
	/// Do not call it yourself, it's done automatically by all rigidbodies.
	/// After that call, the system won't be aware of this rigidbody.
	/// \param body: the rigidbody that will be destroyed
	void unregisterBody(Body * body);

	/// \brief updates the system for one frame.
	void update();

	/// \brief Finds the first collider at the given position in world space
	/// \param point: point that something might collide with
	/// \param except: a collider to exclude, nullptr to not ignore
	/// \return the first collider found.
	ACollider * colliderAt(const sf::Vector2f & point, const ACollider * except=nullptr) const;

	/// \brief Finds the first collider intersecting the given rectangle in world space
	/// \param rect: hitbox that something might collide with
	/// \param except: a collider to exclude, nullptr to not ignore. Useful if the specified hitbox is a collider itself.
	/// \return the first collider found.
	ACollider * colliderAt(const sf::FloatRect & rect, const ACollider * except=nullptr) const;

private:

	/// \brief All colliders in the scene.
	ComponentList<ACollider> m_colliders;

	/// \brief All rigidbodies in the scene.
	ComponentList<Body> m_bodies;

};

} // namespace zn

#endif // HEADER_ZN_PHYSICSSYSTEM_HPP_INCLUDED

