/*
Scene.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>
#include <sstream>
#include <cassert>
#include <map>
#include "Scene.hpp"
#include "graphics/Renderer.hpp"
#include "physics/Collider.hpp"
#include "physics/Body.hpp"

#define ZN_PROFILE_SCENE

namespace zn
{

Scene::Scene() :
	r_mainCamera(nullptr)
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
		ss << "entity" << m_entities.size();
		name = ss.str();
	}
	e->setName(name);

	e->setScene(this);

	m_entities.push_back(e);

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
// Deletes the given entity if its DESTROY_LATE flag is true,
// and returns the flag value
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

//------------------------------------------------------------------------------
void Scene::update(sf::Time deltaTime)
{
	m_deltaTime = deltaTime;

	// Update behaviours in their update order
	for(u32 i = 0; i < m_behaviours.size(); ++i)
	{
		m_behaviours[i].update();
	}

	// Destroy entities with DESTROY_LATE
	m_entities.remove_if(isLateDestroyThenDelete);

	// Update physics
	bodies.update();

	// Update cameras
	cameras.update();

	// Update animations
	animators.update();

	// Update sound
	audioSystem.update(deltaTime);

	// Update renderers
	renderers.update();
}

//------------------------------------------------------------------------------
void Scene::clear()
{
	for(Entity *& e : m_entities)
	{
		// Note: the destruction of an entity triggers the destruction of its
		// components too
		delete e;
	}
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

	// If there is no camera to render the scene, there is nothing to draw
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

	// For each camera
	for(auto cameraIt = cameras.cbegin(); cameraIt != cameras.cend(); ++cameraIt)
	{
		const Camera & camera = **cameraIt;

		// Set view transform
		target.setView(camera.internalView());

		u32 layerMask = camera.entity().layerMask();

		// TODO optimize renderer sorting

		// Filter and sort renderers by draw order
		std::multimap<s32, const ARenderer*> drawList;
		for(auto it = renderers.cbegin(); it != renderers.cend(); ++it)
		{
			const ARenderer * renderer = *it;
			const Entity & entity = renderer->entity();

			if(entity.active() && (entity.layerMask() & layerMask))
			{
				drawList.insert(std::make_pair(renderer->drawOrder, renderer));
			}
		}

		// Draw filtered renderers in the right order
		for(auto it = drawList.cbegin(); it != drawList.cend(); ++it)
		{
			const ARenderer & renderer = *it->second;

			// Apply entity's transform
			states.transform.combine(renderer.entity().m_transform.getTransform());

			// Draw renderer
			target.draw(renderer);
		}

#ifdef ZN_DEBUG
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3))
		{
			// Draw collider boundaries
			for(auto it = drawList.cbegin(); it != drawList.cend(); ++it)
			{
				const ACollider * collider = it->second->entity().collider();
				if(collider != nullptr)
				{
					collider->debug_draw(target);
				}
			}
		}
#endif
	}

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






