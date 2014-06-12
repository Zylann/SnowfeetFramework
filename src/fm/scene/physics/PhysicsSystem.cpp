#include <fm/scene/physics/PhysicsSystem.hpp>
#include <fm/scene/core/Entity.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void PhysicsSystem::registerCollider(Collider* collider)
{
	assert(collider != nullptr);
	m_colliders.add(collider);
}

//------------------------------------------------------------------------------
void PhysicsSystem::unregisterCollider(Collider* collider)
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
Collider * PhysicsSystem::colliderAt(const sf::Vector2f & point, const Collider * except) const
{
	for(auto it = m_colliders.cbegin(); it != m_colliders.cend(); ++it)
	{
		Collider * c = *it;
		if(c != except && c->enabled() && c->entity().activeInHierarchy())
		{
			if(c->collides(point))
				return c;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
Collider * PhysicsSystem::colliderAt(const sf::FloatRect & rect, const Collider * except) const
{
	for(auto it = m_colliders.cbegin(); it != m_colliders.cend(); ++it)
	{
		Collider * c = *it;
		if(c != except && c->enabled() && c->entity().activeInHierarchy())
		{
			if(c->collides(rect))
				return c;
		}
	}
	return nullptr;
}

} // namespace zn


