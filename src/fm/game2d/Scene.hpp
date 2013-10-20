/*
Scene.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_SCENE_HPP_INCLUDED
#define HEADER_ZN_SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Entity.hpp"
#include "LayerMap.hpp"
#include "ComponentSystem.hpp"
#include "components/Behaviour.hpp"

namespace zn
{

// Top-level class for every game object.
// It contains all stuff that the player can see and interact with.
// Only one scene should be active at a time (except for asynchronous loading).
class Scene : public sf::Drawable
{
public:

	Scene();
	~Scene();

	//----------------------------
	// Lifecycle
	//----------------------------

	// Updates the scene for a given time span
	void update(sf::Time deltaTime);

	// Erases everything in the scene
	void clear();

	//----------------------------
	// State & entities
	//----------------------------

	// Creates an empty entity, adds it to the scene and returns it.
	// Note: by default, the update and draw order are defined by their creation order.
	Entity * createEntity(std::string name="", sf::Vector2f pos = sf::Vector2f());

	inline void setMainCamera(Camera * cam) { r_mainCamera = cam; }
	inline Camera * mainCamera() { return r_mainCamera; }

	//----------------------------
	// Events
	//----------------------------

	void onScreenResized(sf::Vector2u resolution);

	//----------------------------
	// Physics
	//----------------------------

	ACollider * colliderAt(const sf::Vector2f & point, const ACollider * except=nullptr);
	ACollider * colliderAt(const sf::FloatRect & rect, const ACollider * except=nullptr);

	//----------------------------
	// Metadata
	//----------------------------

	// Convenient layer name associations
	LayerMap layers;

	inline sf::Time time() const { return m_time.getElapsedTime(); }
	inline sf::Time deltaTime() const { return m_deltaTime; }

	//----------------------------
	// Component systems
	//----------------------------

	void registerBehaviour(ABehaviour * behaviour);
	void unregisterBehaviour(ABehaviour * behaviour);

	ComponentSystem<Camera>         cameras;
	ComponentSystem<ACollider>      colliders;
	ComponentSystem<ARenderer>      renderers;
	ComponentSystem<Body>           bodies;
	ComponentSystem<AAnimator>      animators;

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	std::list<Entity*>              m_entities; // Entities in play

	// References to behaviours [updateOrder][index]
	std::vector< ComponentSystem<AComponent> > m_behaviours;

	Camera *                        r_mainCamera;
	sf::Clock                       m_time;
	sf::Time                        m_deltaTime;

};

} // namespace zn

#endif // HEADER_ZN_SCENE_HPP_INCLUDED



