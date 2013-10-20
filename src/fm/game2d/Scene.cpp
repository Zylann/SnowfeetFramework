/*
Scene.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>
#include <sstream>
#include <cassert>
#include "Scene.hpp"
#include "components/Renderer.hpp"
#include "components/Collider.hpp"
#include "components/Body.hpp"

#define ZN_PROFILE_SCENE

namespace zn
{

Scene::Scene() :
	r_mainCamera(nullptr),
	m_iterating(false)
{
	layers.setLayer(0, "default");
}

Scene::~Scene()
{
	clear();
}

//------------------------------------------------------------------------------
Entity * Scene::createEntity(std::string name, sf::Vector2f pos)
{
	Entity * e = new Entity();

	e->setPosition(pos);

	if(name.empty())
	{
		std::stringstream ss;
		ss << "entity" << m_entities.size() + m_nextEntities.size();
		name = ss.str();
	}
	e->setName(name);

	e->setScene(this);

	m_nextEntities.push_back(e);

#ifdef ZN_DEBUG
	std::cout << "D: Scene::addEntity: " << name
		<< " at (" << pos.x << ", " << pos.y << ")" << std::endl;
#endif

	return e;
}

//------------------------------------------------------------------------------
void Scene::registerBehaviour(ABehaviour * behaviour)
{
	assert(behaviour != nullptr);
	assert(behaviour->componentType().group == CG_BEHAVIOUR);

	u32 updateOrder = behaviour->componentType().updateOrder;
	if(updateOrder >= m_behaviours.size())
	{
		m_behaviours.resize(updateOrder+1);
	}
	m_behaviours[updateOrder].registerComponent(behaviour);
}

//------------------------------------------------------------------------------
void Scene::unregisterBehaviour(ABehaviour * behaviour)
{
	m_behaviours[behaviour->componentType().updateOrder].unregisterComponent(behaviour);
}

//------------------------------------------------------------------------------
// Predicate
bool isLateDestroyThenDelete(Entity * e)
{
	if(e->flag(Entity::DESTROY_LATE))
	{
		delete e;
		return true;
	}
	else
		return false;
}

void Scene::update(sf::Time deltaTime)
{
	m_deltaTime = deltaTime;

	m_iterating = true;

	// Update behaviours
	for(u32 i = 0; i < m_behaviours.size(); ++i)
	{
		m_behaviours[i].update();
	}

	// Spawns
	if(!m_nextEntities.empty())
	{
		m_entities.splice(m_entities.end(), m_nextEntities);
		m_nextEntities.clear();
	}

	// Late destructions
	std::list<Entity*> lateDestroyEntities;
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity * e = *it;
		if(e->flag(Entity::DESTROY_LATE))
		{
			lateDestroyEntities.push_back(e);
		}
	}
	if(!lateDestroyEntities.empty())
	{
		m_entities.remove_if(isLateDestroyThenDelete);
	}

	// Update engine component systems
	bodies.update();
	cameras.update();
	animators.update();
	renderers.update();

	m_iterating = false;

//	m_iterating = true;

//	std::list<Entity*> lateDestroyEntities;
//
//	auto it = m_entities.begin();

//	while(it != m_entities.end())
//	{
//		Entity * e = (*it);
//
//		if(e->active())
//		{
//			e->update();
//		}
//
//		if(e->flag(Entity::DESTROY_NOW))
//		{
//			delete e;
//			it = m_entities.erase(it);
//		}
//		else
//		{
//			if(e->flag(Entity::DESTROY_LATE))
//			{
//				lateDestroyEntities.push_back(e);
//			}
//
//			++it;
//		}
//	}

	// Late destructions
//	if(!lateDestroyEntities.empty())
//	{
//		m_entities.remove_if(isLateDestroyThenDelete);
//	}

	// Spawns
//	if(!m_nextEntities.empty())
//	{
//		m_entities.splice(m_entities.end(), m_nextEntities);
//	}
}

//------------------------------------------------------------------------------
void Scene::clear()
{
	if(m_iterating)
	{
#ifdef ZN_DEBUG
		std::cout << "E: Scene::clear: called during iteration, abort." << std::endl;
#endif
		return;
	}

	m_iterating = true;

	for(Entity *& e : m_entities)
	{
		delete e;
	}

	// Note : should be empty already after update()
	for(Entity *& e : m_nextEntities)
	{
		delete e;
	}

	m_iterating = false;
	m_entities.clear();
}

//------------------------------------------------------------------------------
//void Scene::makeCurrent()
//{
//	g_currentSceneRef = this;
//}

//------------------------------------------------------------------------------
//Scene & Scene::current()
//{
//	return *g_currentSceneRef;
//}

//------------------------------------------------------------------------------
void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	sf::Clock profileClock;
#endif

	if(cameras.empty())
	{
#ifdef ZN_DEBUG
		static bool once = true;
		if(once)
		{
			once = false;
			std::cout << "W: Scene::draw: no camera in the scene, nothing to render." << std::endl;
		}
#endif
		return;
	}

	m_iterating = true;

	// For each camera
	for(auto cameraIt = cameras.cbegin(); cameraIt != cameras.cend(); ++cameraIt)
	{
		const Camera & camera = **cameraIt;

		target.setView(camera.internalView());
		u32 layerMask = camera.entity().layerMask();

		// Draw visible entities
		for(auto it = renderers.cbegin(); it != renderers.cend(); ++it)
		{
			const ARenderer & r = **it;

			if(r.entity().active() && (r.entity().layerMask() & layerMask))
			{
				target.draw(r);
				//r.draw(target, states);
			}
		}
	}

	m_iterating = false;

#if defined(ZN_DEBUG) && defined(ZN_PROFILE_SCENE)
	float renderTimeMs = profileClock.getElapsedTime().asMilliseconds();
	static u32 renderTimes = 0;
	if((++renderTimes) % 100 == 0)
	{
		std::cout << "D: Scene: render time: " << renderTimeMs << "ms" << std::endl;
	}
#endif
}

//------------------------------------------------------------------------------
void Scene::onScreenResized(sf::Vector2u resolution)
{
	for(auto it = cameras.begin(); it != cameras.end(); ++it)
	{
		(*it)->onScreenResized(resolution);
	}
}

//------------------------------------------------------------------------------
ACollider * Scene::colliderAt(const sf::Vector2f & point, const ACollider * except)
{
	for(Entity *& e : m_entities)
	{
		ACollider * c = e->collider();
		if(c != nullptr && c != except)
		{
			if(c->collides(point))
				return c;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
ACollider * Scene::colliderAt(const sf::FloatRect & rect, const ACollider * except)
{
	for(Entity *& e : m_entities)
	{
		ACollider * c = e->collider();
		if(c != nullptr && c != except)
		{
			if(c->collides(rect))
				return c;
		}
	}
	return nullptr;
}

} // namespace zn






