/*
Entity.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the Plane-framework project.
*/

#ifndef HEADER_ZN_ENTITY_HPP_INCLUDED
#define HEADER_ZN_ENTITY_HPP_INCLUDED

#include <unordered_map>
#include <SFML/Graphics.hpp>

#include <fm/types.hpp>
#include <fm/proto/animation/Animator.hpp>
#include <fm/proto/graphics/Camera.hpp>
#include <fm/proto/Transform.hpp>
#include <fm/proto/LayerMap.hpp>

namespace zn
{

class AComponent;
class ARenderer;
class ACollider;
class Body;
class Scene;
class AudioEmitter;

/// \brief An entity can be anything in the scene.
/// It is composed of components that define its appearance, interactions and behavior.
class ZN_API Entity
{
public:

	/// \brief Flags that can be set on the entity, as bitmasks.
	enum Flags
	{
		ACTIVE       = 1,
		DESTROY_LATE = 1 << 1, // Destroy at the end of the frame
		CROSS_SCENE  = 1 << 2 // Don't destroy the entity on scene change
	};

	/// \brief Constructs a new empty entity, with no link to a scene.
	/// \warning You usually shouldn't call this constructor, use Scene::createEntity() instead.
	/// \see Scene.hpp
	Entity();

	/// \brief Destroys the entity and all of its components
	~Entity();

	//--------------------------------------
	// Lifecycle
	//--------------------------------------

	/// \brief Tests if the entity alone is active, regardless of its parents
	/// \return true if the entity alone is active, false otherwise
	bool active() const;

	/// \brief Returns true if the entity is active and all of its parents are active.
	/// \return true if the entity is active, false otherwise
	bool activeInHierarchy() const;

	/// \brief Activates or desactivates the entity.
	/// \param active
	void setActive(bool active);

	/// \brief Schedules the destruction of the entity at the end of the current frame.
	/// \note this will also destroy any child entity under this one at _destruction_time_.
	/// For instance, if you unparent a child before the end of the frame, this
	/// child will not be deleted.
	void destroyLater();

	/// \brief Sets if the entity should be destroyed when we change the scene
	void setCrossScene(bool crossScene);

	/// \brief Gets the value of a flag on the entity by providing its mask.
	/// \return wether the flag is set or not
	inline bool flag(u8 mask) const { return m_flags & mask; }

	//--------------------------------------
	// Structure & behavior
	//--------------------------------------

	/// \brief The position, rotation, scale and hierarchy of the entity is stored here.
	Transform transform;

	/// \brief gets the name of the entity.
	/// \return name of the entity.
	inline const std::string & name() const { return m_name; }

	/// \brief Sets the name of the entity. There is no particular restrictions on it,
	/// except that it may be short enough to identifiate the entity well for humans.
	/// \param newName: name to give.
	inline void setName(const std::string & newName) { m_name = newName; }

	/// \brief Adds a native component of the given type to the entity.
	/// \note Note that an entity cannot have two components of the same type.
	/// \return the newly added component, or nullptr if something went wrong.
	template <class Component_T>
	Component_T * addComponent()
	{
		// Get metaclass
		const ComponentType & ct = Component_T::sComponentType();

		// Check if the component can be added
		if(!checkComponentAddition(ct, "Entity::addComponent"))
		{
			return nullptr;
		}

		// Create component
		Component_T * newComponent = new Component_T();

		// Call internal component adder
		addComponent(newComponent);

		return newComponent;
	}

	// TODO template equivalent for removeComponent()

	/// \brief Removes a component from the entity by providing a direct reference to it.
	/// \param cmp: pointer on the component to remove.
	void removeComponent(AComponent * cmp);

	/// \brief Finds the component of the given type attached to this entity.
	/// \return the component, or nullptr if not found.
	template <class Component_T>
	Component_T * getComponent()
	{
		const ComponentType & ct = Component_T::sComponentType();

		auto it = m_components.find(ct.ID);
		if(it != m_components.end())
		{
			return it->second;
		}

		return nullptr;
	}

	/// \brief Returns the entity's renderer, if any.
	inline ARenderer * renderer() const  { return r_renderer; }

	/// \brief Returns the entity's animator, if any.
	inline AAnimator * animator() const  { return r_animator; }

	/// \brief Returns the entity's camera, if any.
	inline Camera * camera() const       { return r_camera; }

	/// \brief Returns the entity's collider, if any.
	inline ACollider * collider() const  { return r_collider; }

	/// \brief Returns the entity's rigidbody, if any.
	inline Body * body() const           { return r_body; }

	/// \brief Returns the entity's audio emitter, if any.
	inline AudioEmitter * audio() const  { return r_audioEmitter; }

	/// \brief Returns the scene the entity belongs to.
	Scene & scene() const;

	// Sends a message to the components of the entity
	void sendMessage(const std::string & msg);

	/// \brief This function is called when the entity enters in collision with something.
	/// \warning this function is not currently called by the engine, because
	/// it has no integrated physics engine yet.
	void onCollisionEnter(const CollisionInfo & info);

	/// \brief This function is called when the entity quits from a collision with something.
	/// \warning this function is not currently called by the engine, because
	/// it has no integrated physics engine yet.
	void onCollisionExit(const CollisionInfo & info);

	//--------------------------------------
	// Layers
	//--------------------------------------

	/// \brief Moves the entity to the specified layer.
	/// \param layer: the name of the layer
	void setLayerByName(const std::string & layer);

	/// \brief Moves the entity to the specified layer.
	/// \param layer: the numeric index of the layer
	void setLayerByIndex(u32 layerIndex);

	/// \brief Gets the index of the layer the entity belongs to.
	/// \return The numeric index of the layer.
	inline u32 layerIndex() const { return m_layerIndex; }

	/// \brief Gets the layer the entity belongs to.
	/// \return Reference to the layer object.
	const Layer & layer() const;

	//--------------------------------------
	// Serialization
	//--------------------------------------

	/// \brief serializes the entity to a JSON data tree.
	/// \param o: JSON object representing the entity
	void serialize(JsonBox::Value & o);

	/// \brief deserializes the entity from a JSON data tree.
	/// \param o: JSON object representing the entity
	void unserialize(JsonBox::Value & o);

	/// \brief must be called after the entity to be deserialized into a scene,
	/// in order to relink references and perform some checks
	/// \param o: JSON object representing the entity, to pick the references data
	void postUnserialize(JsonBox::Value & o);

	/// \brief Gets the unique numerical ID of this entity
	/// \return Numerical ID of the entity
	inline u32 id() const { return m_id; }

private:

	friend class Scene;

	/// \brief Sets the scene the entity belongs to.
	/// Usually, only the scene has to call it once when the entity is created.
	inline void setScene(Scene * scene)
	{
		r_scene = scene;
	}

	/// \brief Internal flag setter.
	/// \warning this function may be removed in the future (using std::bitset).
	inline void setFlag(u8 mask, bool flag)
	{
		if(flag)
			m_flags |= mask;
		else
			m_flags &= ~mask;
	}

	/// \brief Adds a component to the entity. Internal use.
	/// \param newCmp: newly created component
	/// \return the component passed in parameter.
	AComponent * addComponent(AComponent * newCmp);

	/// \brief Checks a component type before adding a component instance.
	/// Prints debug messages if the check fails.
	/// \param ct: meta-class of the component to add
	/// \param context: contextual information to give if an error occurs (ie. the calling function).
	/// \return true if the component can be added, false otherwise
	bool checkComponentAddition(const ComponentType & ct, const std::string & context);

	//--------------------------------------
	// Attributes
	//--------------------------------------

	/// \brief Unique numerical ID.
	/// It is generated when the entity is created and added to a scene.
	/// \see Scene.cpp
	u32 m_id;

	/// \brief Components attached to this entity, stored by type.
	std::unordered_map<ComponentTypeID, AComponent*>  m_components;

	/// \brief direct reference to the renderer for quick lookup
	ARenderer * r_renderer;

	/// \brief direct reference to the collider for quick lookup
	ACollider * r_collider;

	/// \brief direct reference to the rigidbody for quick lookup
	Body * r_body;

	/// \brief direct reference to the camera for quick lookup
	Camera * r_camera;

	/// \brief direct reference to the animator for quick lookup
	AAnimator * r_animator;

	/// \brief direct reference to the audio emitter for quick lookup
	AudioEmitter * r_audioEmitter;

	/// \brief Bitset containing boolean states of the entity
	u8 m_flags; // TODO use std::bitset

	/// \brief Numerical index of the layer the entity belongs to
	u32 m_layerIndex;

	/// \brief Scene the entity belongs to
	Scene * r_scene;

	/// \brief User-defined name of the entity
	std::string m_name;

};

} // namespace zn

#endif // HEADER_ZN_ENTITY_HPP_INCLUDED


