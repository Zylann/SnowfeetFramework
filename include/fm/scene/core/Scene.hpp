/*
Scene.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the Plane Framework project.
*/

#ifndef HEADER_ZN_SCENE_HPP_INCLUDED
#define HEADER_ZN_SCENE_HPP_INCLUDED

#include <list>
#include <unordered_set>
#include <SFML/Graphics.hpp>

#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/LayerMap.hpp>
#include <fm/scene/core/ComponentList.hpp>
#include <fm/scene/behaviour/Behaviour.hpp>
#include <fm/scene/audio/AudioSystem.hpp>
#include <fm/scene/graphics/RenderSystem.hpp>
#include <fm/scene/physics/PhysicsSystem.hpp>
#include <fm/scene/core/TagManager.hpp>
#include <fm/scene/gui/GUIManager.hpp>

namespace zn
{

/// \brief A scene contains all stuff that the player can see and interact with.
/// Only one scene should be active at a time.
/// Disclaimer: despite being an sf::Drawable, the scene is not dedicated to graphics,
/// it simply manages the entity list and specialized subsystems
/// (sound, graphics, physics, behaviour...).
class ZN_API Scene : public sf::Drawable, public IInputListener
{
public:

	Scene();
	~Scene();

	//----------------------------
	// Lifecycle
	//----------------------------

	// TODO add scene modes : EDIT / PLAY / RECORD

	/// \brief Updates the scene for a given time span.
	void update(sf::Time deltaTime);

	/// \brief Erases everything in the scene.
	void clear();

	/// \brief Erases everything in the scene but entities having the EF_SCENE_CROSS flag set.
	void clearAllButCrossEntities();

	//----------------------------
	// State & entities
	//----------------------------

	/// \brief Creates an empty entity, adds it to the scene and returns it.
	/// \note: by default, the update and draw order are defined by their creation order.
	Entity * createEntity(std::string name="", sf::Vector2f pos = sf::Vector2f());

	/// \brief Sets the given camera as the main camera
	inline void setMainCamera(Camera * cam) { r_mainCamera = cam; }

	/// \brief Gets the main camera (if defined).
	inline Camera * mainCamera() { return r_mainCamera; }

	/// \brief Finds the entity having the given ID.
	/// \param id: numeric unique identifier of the entity within the scene
	/// \return Returns nullptr when not found.
	Entity * findEntityFromID(u32 id) const;

	/// \brief Finds the first encountered entity having the given name.
	/// \param name: name to search
	/// \return nullptr when not found.
	Entity * findEntityFromName(const std::string & name);

	//----------------------------
	// Events
	//----------------------------

	/// \brief Called each time the screen resolution changes
	/// \param resolution: new resolution in pixels
	void onScreenResized(sf::Vector2u resolution);

	//----------------------------
	// Metadata
	//----------------------------

	/// \brief Convenient layer name associations
	LayerMap layers;

	/// \brief Tag definitions
	TagManager tagManager;

	/// \brief Returns the amount of time elapsed since the scene was loaded
	inline sf::Time time() const { return m_time.getElapsedTime(); }

	/// \brief Returns the duration between two frames
	inline sf::Time deltaTime() const { return m_deltaTime; }

	//----------------------------
	// Serialization
	//----------------------------

	/// \brief saves the scene in a JSON data tree.
	void serialize(JsonBox::Value & o);
	/// \brief loads the scene from a JSON data tree.
	void unserialize(JsonBox::Value & o);

	/// \brief saves the scene in a JSON file.
	/// \param filePath: path to the file. If it exists, it is overwritten. If it doesn't exists, it is created.
	/// \return true if success, false if an error occurred.
	bool saveToFile(const std::string & filePath);
	/// \brief loads the scene from a JSON file.
	/// \param filePath: path to the file.
	/// \return true if success, false if the file didn't exist or had invalid structure.
	bool loadFromFile(const std::string & filePath);

	//----------------------------
	// Component systems
	//----------------------------

	/// \brief Internal: called when a behaviour has been created
	void registerBehaviour(Behaviour * behaviour);

	/// \brief Internal: called just before a behaviour to be deleted
	void unregisterBehaviour(Behaviour * behaviour);

	/// \brief All the animators in the scene.
	ComponentList<Animator> animators;
	/// \brief Manages physics in the scene (all colliders, all bodies).
	PhysicsSystem physics;
	/// \brief Controls global audio in the scene (also references AudioEmitters).
	AudioSystem audioSystem;
	/// \brief Everything visual (list of renderers and cameras).
	RenderSystem renderSystem;
	/// \brief GUI system.
	GUIManager guiManager;

protected:

	/// \brief draws the entire scene on the given target.
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	/// \brief counter used to generate entity UIDs
	u32 m_nextID;

	/// \brief list of all entities in the scene.
	std::list<Entity*> m_entities;

	/// \brief References to all behaviours [updateOrder][index]
	std::map< s32, ComponentList<Behaviour> > m_behaviours;

	/// \brief reference to the main camera
	Camera * r_mainCamera;

	/// \brief time elapsed since the scene was loaded
	sf::Clock m_time;

	/// \brief Cached current frame delta time
	sf::Time m_deltaTime;

};

} // namespace zn

#endif // HEADER_ZN_SCENE_HPP_INCLUDED



