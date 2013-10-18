#include <iostream>
#include "Entity.hpp"
#include "components/Component.hpp"
#include "components/Renderer.hpp"
#include "components/Collider.hpp"
#include "components/Body.hpp"
#include "Scene.hpp"
#include "../util/typecheck.hpp"

namespace zn
{

Entity::Entity() :
	r_renderer(nullptr),
	r_collider(nullptr),
	r_body(nullptr),
	r_camera(nullptr),
	r_animator(nullptr),
	m_flags(ACTIVE),
	m_layerMask(1),
	r_scene(nullptr)
{
	setName("_entity_");
}

Entity::~Entity()
{
	for(AComponent *& cmp : m_components)
	{
		cmp->onDestroy();
		delete cmp;
	}
}

//------------------------------------------------------------------------------
Scene & Entity::scene() const
{
	return *r_scene;
}

//------------------------------------------------------------------------------
void Entity::setScale(const sf::Vector2f & s)
{
	m_transform.setScale(s);
}

//------------------------------------------------------------------------------
void Entity::setPosition(const sf::Vector2f& pos)
{
	m_transform.setPosition(pos);
}

//------------------------------------------------------------------------------
void Entity::setRotation(float angleDeg)
{
	m_transform.setRotation(angleDeg);
}

//------------------------------------------------------------------------------
/*
void Entity::init()
{
	// Initialize components
	for(AComponent *& cmp : m_components)
		cmp->init();
}
*/
//------------------------------------------------------------------------------
/*
void Entity::flushComponents()
{
	if(!m_nextBehaviours.empty())
	{
		// Initialize components that have been added before
		for(u32 i = 0; i < m_nextBehaviours.size(); ++i)
		{
			m_nextBehaviours[i]->init();
			m_behaviours.push_back(m_nextBehaviours[i]); // And add them to the main list
		}
		m_nextBehaviours.clear();
		//_behaviours.splice(_behaviours.end(), _nextBehaviours);
	}
}
*/
//------------------------------------------------------------------------------
//void Entity::update()
//{
//	flushComponents();

	// Update components
//	for(u32 i = 0; i < m_behaviours.size(); ++i)
//	{
//		m_components[i]->update();
//	}

//	flushComponents();

//	if(m_body != nullptr)
//		m_body->update();
//
//	if(m_renderer != nullptr)
//		m_renderer->update();
//}

//------------------------------------------------------------------------------
void Entity::destroyLater()
{
	setFlag(DESTROY_LATE, true);
}

//------------------------------------------------------------------------------
bool Entity::active() const
{
	return m_flags & ACTIVE;
}

//------------------------------------------------------------------------------
void Entity::setActive(bool active)
{
	setFlag(ACTIVE, active);
}

//------------------------------------------------------------------------------
AComponent * Entity::addComponent(AComponent * newCmp)
{
#ifdef ZN_DEBUG
	if(newCmp == nullptr)
		std::cout << "E: Entity::addComponent: newCmp is null !" << std::endl;
#endif
	assert(newCmp != nullptr);

	const ComponentType & ct = newCmp->componentType();

#ifdef ZN_DEBUG
	// Check component group unicity
	if(ct.flags & CTF_UNIQUE_OF_GROUP)
	{
		bool duplicate = false;
		for(u32 i = 0; i < m_components.size(); ++i)
		{
			if(m_components[i]->componentType().group == ct.group);
				duplicate = true;
		}
		if(duplicate)
		{
			std::cout << "E: Entity::addComponent: duplicate, "
				"the component " << ct.name << " must be unique per entity." << std::endl;
		}
		assert(!duplicate);
	}
#endif

	// Assign quick lookup pointer
	switch(ct.group)
	{
	case CG_RENDERER:  r_renderer = checked_cast<ARenderer*>(newCmp); break;
	case CG_COLLIDER:  r_collider = checked_cast<ACollider*>(newCmp); break;
	case CG_CAMERA:    r_camera   = checked_cast<Camera*>(newCmp); break;
	case CG_BODY:      r_body     = checked_cast<Body*>(newCmp); break;
	case CG_ANIMATOR:  r_animator = checked_cast<AAnimator*>(newCmp); break;
	default: break;
	}

	newCmp->onAdd(this);

	newCmp->init();

	return newCmp;
}

//------------------------------------------------------------------------------
/*
void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if(m_renderer != nullptr)
	{
		states.transform.combine(m_transform.getTransform());
		target.draw(*m_renderer, states);

#ifdef ZN_DEBUG
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3))
		{
			const ACollider * c = collider();
			if(c != nullptr)
			{
				c->debug_draw(target);
			}
		}
#endif
	}
}
*/
//------------------------------------------------------------------------------
void Entity::sendMessage(const std::string & msg)
{
	for(u32 i = 0; i < m_components.size(); ++i)
	{
		m_components[i]->onMessage(msg);
	}

	if(r_renderer != nullptr)
		r_renderer->onMessage(msg);
}

//------------------------------------------------------------------------------
void Entity::onCollisionEnter(const CollisionInfo & info)
{
	for(u32 i = 0; i < m_components.size(); ++i)
	{
		m_components[i]->onCollisionEnter(info);
	}
}

//------------------------------------------------------------------------------
void Entity::onCollisionExit(const CollisionInfo & info)
{
	for(u32 i = 0; i < m_components.size(); ++i)
	{
		m_components[i]->onCollisionExit(info);
	}
}

} // namespace zn





