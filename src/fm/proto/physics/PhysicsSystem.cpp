#include <fm/proto/physics/PhysicsSystem.hpp>
#include <fm/proto/core/Entity.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void PhysicsSystem::registerCollider(ACollider* collider)
{
	assert(collider != nullptr);
	m_colliders.add(collider);
}

//------------------------------------------------------------------------------
void PhysicsSystem::unregisterCollider(ACollider* collider)
{
	m_colliders.remove(collider);
}

//------------------------------------------------------------------------------
void PhysicsSystem::registerBody(Body* body)
{
	assert(body != nullptr);
	m_bodies.add(body);
}

//------------------------------------------------------------------------------
void PhysicsSystem::unregisterBody(Body* body)
{
	m_bodies.remove(body);
}

//------------------------------------------------------------------------------
void PhysicsSystem::update()
{
	//m_colliders.update();
	//m_bodies.update();
}

//------------------------------------------------------------------------------
ACollider * PhysicsSystem::colliderAt(const sf::Vector2f & point, const ACollider * except) const
{
	for(auto it = m_colliders.cbegin(); it != m_colliders.cend(); ++it)
	{
		ACollider * c = *it;
		if(c != except && c->enabled() && c->entity().activeInHierarchy())
		{
			if(c->collides(point))
				return c;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
ACollider * PhysicsSystem::colliderAt(const sf::FloatRect & rect, const ACollider * except) const
{
	for(auto it = m_colliders.cbegin(); it != m_colliders.cend(); ++it)
	{
		ACollider * c = *it;
		if(c != except && c->enabled() && c->entity().activeInHierarchy())
		{
			if(c->collides(rect))
				return c;
		}
	}
	return nullptr;
}

} // namespace zn


