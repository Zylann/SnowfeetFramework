/*
Entity.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_ENTITY_HPP_INCLUDED
#define HEADER_ZN_ENTITY_HPP_INCLUDED

#include <list>
#include <SFML/Graphics.hpp>

#include <fm/types.hpp>
#include <fm/proto/animation/Animator.hpp>
#include <fm/proto/graphics/Camera.hpp>
#include <fm/proto/Transform.hpp>

namespace zn
{

class AComponent;
class ARenderer;
class ACollider;
class Body;
class Scene;
class AudioEmitter;

// An entity can be anything in the scene.
// It is composed of components that define its behavior.
class ZN_API Entity
{
public:

	enum Flags
	{
		ACTIVE       = 1,
		DESTROY_LATE = 1 << 1, // Destroy at the end of the frame
		CROSS_SCENE  = 1 << 2 // Don't destroy the entity on scene change
	};

	Entity();
	~Entity();

	//--------------------------------------
	// Lifecycle
	//--------------------------------------

	// Returns true if the entity alone is active, regardless of its parents
	bool active() const;

	// Returns true if the entity is active and all of its parents are active.
	bool activeInHierarchy() const;

	void setActive(bool active);

	// Schedules the destruction of the entity at the end of the current frame.
	// Note: this will also destroy any child entity under this one at _destruction_time_.
	// For instance, if you unparent a child before the end of the frame, this
	// child will not be deleted.
	void destroyLater();

	// Sets if the entity should be destroyed when we change the scene
	void setCrossScene(bool crossScene);

	// Gets a flag of the entity by providing its mask
	inline bool flag(u8 mask) const { return m_flags & mask; }

	//--------------------------------------
	// Structure & behavior
	//--------------------------------------

	Transform transform;

	inline const std::string & name() const { return m_name; }
	inline void setName(const std::string & newName) { m_name = newName; }

	template <class Cmp_T>
	Cmp_T * addComponent()
	{
		Cmp_T * cmp = new Cmp_T;
		if(addComponent(cmp) != nullptr)
			return cmp;
		else
		{
			delete cmp;
			return nullptr;
		}
	}

	void removeComponent(AComponent * cmp);

	// Finds the first behaviour matching the given type.
	// Warning: this operation is very slow. You may use it only if it's the only way.
	template <class Cmp_T>
	Cmp_T * getComponent()
	{
		Cmp_T * cmpt;
		for(AComponent *& cmp : m_components)
		{
			if((cmpt = dynamic_cast<Cmp_T*>(cmp)) != nullptr)
				return cmpt;
		}
		return nullptr;
	}

	// Shortcuts
	inline ARenderer * renderer() const  { return r_renderer; }
	inline AAnimator * animator() const  { return r_animator; }
	inline Camera * camera() const       { return r_camera; }
	inline ACollider * collider() const  { return r_collider; }
	inline Body * body() const           { return r_body; }
	inline AudioEmitter * audio() const  { return r_audioEmitter; }

	// Returns the scene the entity belongs to.
	Scene & scene() const;

	// Sends a message to the components of the entity
	void sendMessage(const std::string & msg);

	void onCollisionEnter(const CollisionInfo & info);
	void onCollisionExit(const CollisionInfo & info);

	//--------------------------------------
	// Layers
	//--------------------------------------

	inline void setLayerMask(u32 mask) { m_layerMask = mask; }
	inline u32 layerMask() const { return m_layerMask; }

	//--------------------------------------
	// Serialization
	//--------------------------------------

	void serialize(JsonBox::Value & o);
	void unserialize(JsonBox::Value & o);
	void postUnserialize(JsonBox::Value & o);

	inline u32 id() const { return m_id; }

private:

	friend class Scene;

	// Sets the scene the entity belongs to.
	// Usually, only the scene has to call it.
	inline void setScene(Scene * scene)
	{
		r_scene = scene;
	}

	// Internal flag setter
	inline void setFlag(u8 mask, bool flag)
	{
		if(flag)
			m_flags |= mask;
		else
			m_flags &= ~mask;
	}

	// Internal method for adding a component
	AComponent * addComponent(AComponent * newCmp);

	//--------------------------------------
	// Attributes
	//--------------------------------------

	u32                       m_id;

	std::vector<AComponent*>  m_components;

	// References to engine components for quick lookup
	ARenderer *               r_renderer;
	ACollider *               r_collider;
	Body *                    r_body;
	Camera *                  r_camera;
	AAnimator *               r_animator;
	AudioEmitter *            r_audioEmitter;

	u8                        m_flags;
	u32                       m_layerMask;

	Scene *                   r_scene;

	std::string               m_name;

};

} // namespace zn

#endif // HEADER_ZN_ENTITY_HPP_INCLUDED


