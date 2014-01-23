/*
Scene.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <map>

#include <fm/proto/Scene.hpp>
#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/physics/Collider.hpp>
#include <fm/proto/physics/Body.hpp>

#define ZN_PROFILE_SCENE

namespace zn
{

Scene::Scene() :
	m_nextID(0),
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

	e->m_id = m_nextID++;

	e->transform.setPosition(pos);

	if(name.empty())
	{
		std::stringstream ss;
		ss << "entity" << e->id();
		name = ss.str();
	}
	e->setName(name);

	e->setScene(this);

	m_entities.push_back(e);

#ifdef ZN_DEBUG
	std::cout << "D: Scene::addEntity: [" << e->id() << "] " << name
		<< " at (" << pos.x << ", " << pos.y << ")" << std::endl;
#endif

	return e;
}

//------------------------------------------------------------------------------
Entity * Scene::findEntityFromID(u32 id) const
{
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if((*it)->id() == id)
			return *it;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
Entity * Scene::findEntityFromName(const std::string & name)
{
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if((*it)->name() == name)
			return *it;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
void Scene::registerBehaviour(ABehaviour * behaviour)
{
	assert(behaviour != nullptr);
	assert(behaviour->componentType().group == CG_BEHAVIOUR);

	s32 updateOrder = behaviour->componentType().updateOrder;
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
	for(auto it = m_behaviours.begin(); it != m_behaviours.end(); ++it)
	{
		it->second.update();
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

	m_nextID = 0;
}

//------------------------------------------------------------------------------
// Deletes the given entity if its CROSS_SCENE flag is false.
// Returns true if the entity has been deleted.
bool isNotSceneCrossThenDelete(Entity * e)
{
	if(!e->flag(Entity::CROSS_SCENE))
	{
		delete e;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void Scene::clearAllButCrossEntities()
{
	m_entities.remove_if(isNotSceneCrossThenDelete);
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
bool f_cameraOrder(const Camera *&cam1, const Camera *&cam2)
{
    return cam1->depth < cam2->depth;
}

//------------------------------------------------------------------------------
bool f_rendererOrder(const ARenderer *&r1, const ARenderer *&r2)
{
    return r1->drawOrder < r2->drawOrder;
}

//------------------------------------------------------------------------------
void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG

	sf::Clock profileClock;

	bool drawBounds = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9);
	bool drawColliders = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10);

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

	// Filter and sort cameras by depth
	std::list<const Camera*> sortedCameras;
	for(auto cameraIt = cameras.cbegin(); cameraIt != cameras.cend(); ++cameraIt)
	{
	    const Camera * camera = *cameraIt;
	    if(camera->entity().active())
        {
            sortedCameras.push_back(camera);
        }
	}
	sortedCameras.sort(f_cameraOrder);

	// For each active camera
	for(auto cameraIt = sortedCameras.cbegin(); cameraIt != sortedCameras.cend(); ++cameraIt)
	{
		const Camera & camera = **cameraIt;

		// Set view transform
		target.setView(camera.internalView());

		u32 layerMask = camera.entity().layerMask();

		// Filter and sort renderers by draw order
		std::list<const ARenderer*> drawList;
		for(auto it = renderers.cbegin(); it != renderers.cend(); ++it)
		{
			const ARenderer * renderer = *it;
			const Entity & entity = renderer->entity();

			if(entity.active() && (entity.layerMask() & layerMask))
			{
				drawList.push_back(renderer);
			}
		}
		drawList.sort(f_rendererOrder);

		// Draw filtered renderers in the right order
		for(auto it = drawList.cbegin(); it != drawList.cend(); ++it)
		{
			const ARenderer & renderer = **it;

			// Draw renderer
			target.draw(renderer);

#ifdef ZN_DEBUG
			if(drawBounds)
			{
				// Draw renderer bounds
				// TODO use global bounds when they will be implemented
				sf::FloatRect bounds = renderer.localBounds();
				sf::RectangleShape rect(sf::Vector2f(bounds.width, bounds.height));
				rect.setFillColor(sf::Color::Transparent);
				rect.setOutlineColor(sf::Color(64,64,255));
				rect.setPosition(renderer.entity().transform.position());
				rect.setOutlineThickness(1);
				target.draw(rect);
			}

			if(drawColliders)
			{
				// Draw collider boundaries
				const ACollider * collider = (*it)->entity().collider();
				if(collider != nullptr)
				{
					collider->debug_draw(target);
				}
			}
#endif
		}
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

//------------------------------------------------------------------------------
void Scene::serialize(JsonBox::Value & o)
{
	JsonBox::Array entityListData;
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		JsonBox::Value entityData;
		(*it)->serialize(entityData);
		entityListData.push_back(entityData);
	}

	o["nextID"] = (s32)m_nextID;
	o["entities"] = entityListData;
}

//------------------------------------------------------------------------------
void Scene::unserialize(JsonBox::Value & o)
{
	// Unserialized content will replace old content
	clear();

	u32 computedNextID = 0;

	std::unordered_map<u32,Entity*> id2entity;
	std::list<std::pair<Entity*,JsonBox::Value*>> entityDataList;

	JsonBox::Value & entityListData = o["entities"];
	u32 n = entityListData.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		JsonBox::Value & entityData = entityListData[i];

		Entity * entity = createEntity();
		u32 id = entityData["id"].getInt();

		auto it = id2entity.find(id);
		if(it != id2entity.end())
		{
			std::cout << "E: Conflicting entity found on scene loading : "
				"\"" << entityData["name"] << "\"[" << id << "] "
				"conflicts with "
				"\"" << it->second->name() << "\"[" << it->second->id() << "]"
				<< std::endl;
			continue;
		}
		else
		{
			id2entity.insert(std::make_pair(id, entity));
		}

		entity->unserialize(entityListData[i]);

		entityDataList.push_back(std::make_pair(entity, &entityData));

		if(computedNextID <= id)
		{
			computedNextID = id+1;
		}
	}

	m_nextID = computedNextID;

	// Post-unserialize

	for(auto it = entityDataList.begin(); it != entityDataList.end(); ++it)
	{
		Entity * entity = it->first;
		JsonBox::Value * entityData = it->second;

		entity->postUnserialize(*entityData);
	}
}

//------------------------------------------------------------------------------
bool Scene::saveToFile(const std::string & filePath)
{
#ifdef ZN_DEBUG
	std::cout << "D: Saving scene as \"" << filePath << '"' << std::endl;
	sf::Clock timer;
#endif

	std::ofstream ofs(filePath.c_str(), std::ios::out|std::ios::trunc|std::ios::binary);
	if(ofs.good())
	{
		JsonBox::Value v;
		serialize(v);
		v.writeToStream(ofs);
		ofs.close();
	}
	else
	{
		std::cout << "E: Failed to load scene from \"" << filePath << '"' << std::endl;
		return false;
	}

#ifdef ZN_DEBUG
	f32 timeSpent = timer.getElapsedTime().asSeconds();
	std::cout << "D: Took " << timeSpent << "s to serialize the scene as a file." << std::endl;
#endif
	return true;
}

//------------------------------------------------------------------------------
bool Scene::loadFromFile(const std::string & filePath)
{
#ifdef ZN_DEBUG
	std::cout << "D: Loading scene from \"" << filePath << '"' << std::endl;
	sf::Clock timer;
#endif

	std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
	if(ifs.good())
	{
		JsonBox::Value v;
		unserialize(v);
		v.loadFromStream(ifs);
		ifs.close();
	}
	else
	{
		std::cout << "E: Failed to save scene to \"" << filePath << '"' << std::endl;
		return false;
	}

#ifdef ZN_DEBUG
	f32 timeSpent = timer.getElapsedTime().asSeconds();
	std::cout << "D: Took " << timeSpent << "s to serialize the scene as a file." << std::endl;
#endif
	return true;
}

} // namespace zn






