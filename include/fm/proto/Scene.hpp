/*
Scene.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_SCENE_HPP_INCLUDED
#define HEADER_ZN_SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <unordered_set>

#include <fm/proto/Entity.hpp>
#include <fm/proto/LayerMap.hpp>
#include <fm/proto/ComponentSystem.hpp>
#include <fm/proto/behaviour/Behaviour.hpp>
#include <fm/proto/audio/AudioSystem.hpp>

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

	// Finds the entity having the given ID.
	// Returns nullptr when not found.
	Entity * findEntityFromID(u32 id) const;

	// Finds the first encountered entity having the given name.
	// Returns nullptr when not found.
	Entity * findEntityFromName(const std::string & name);

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
	// Serialization
	//----------------------------

	void serialize(JsonBox::Value & o);
	void unserialize(JsonBox::Value & o);

	bool saveToFile(const std::string & filePath);
	bool loadFromFile(const std::string & filePath);

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
	AudioSystem                     audioSystem;

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	u32                             m_nextID;

	// TODO add scene modes : EDIT / PLAY / RECORD

	std::list<Entity*>              m_entities; // Entities in play

	// References to behaviours [updateOrder][index]
	std::map< s32, ComponentSystem<ABehaviour> > m_behaviours;

	Camera *                        r_mainCamera;
	sf::Clock                       m_time;
	sf::Time                        m_deltaTime;

};

} // namespace zn

#endif // HEADER_ZN_SCENE_HPP_INCLUDED



