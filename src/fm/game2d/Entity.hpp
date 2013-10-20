/*
Entity.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_ENTITY_HPP_INCLUDED
#define HEADER_ZN_ENTITY_HPP_INCLUDED

#include <list>
#include <SFML/Graphics.hpp>
#include "../types.hpp"
#include "components/Animator.hpp"
#include "components/Camera.hpp"
#include "Object.hpp"

namespace zn
{

class AComponent;
class ARenderer;
class ACollider;
class Body;
class Scene;

// An entity can be anything in the scene.
// It is composed of components that define its behavior.
class Entity : public Object
{
public:

	enum Flags
	{
		ACTIVE       = 1,
		DESTROY_LATE = 1 << 1
	};

	Entity();
	~Entity();

	//--------------------------------------
	// Transform wrapped getters
	//--------------------------------------

	inline const sf::Vector2f & position() const { return m_transform.getPosition(); }
	inline const sf::Vector2f & scale() const { return m_transform.getScale(); }
	inline f32 rotation() const { return m_transform.getRotation(); }

	//--------------------------------------
	// Transform wrapped setters
	//--------------------------------------

	void setPosition(const sf::Vector2f & pos);
	void setScale(const sf::Vector2f & s);
	void setRotation(float angleDeg);

	//--------------------------------------
	// Lifecycle
	//--------------------------------------

//	void init();
//	void update();

	bool active() const;
	void setActive(bool active);

	// Schedules the destruction of the entity at the end of the current frame
	void destroyLater();

	// Gets a flag of the entity by providing its mask
	inline bool flag(u8 mask) const { return m_flags & mask; }

	//--------------------------------------
	// Structure & behavior
	//--------------------------------------

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

	// TODO eraseComponent(cmp)

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

	inline ARenderer * renderer() const  { return r_renderer; }
	inline AAnimator * animator() const  { return r_animator; }
	inline Camera * camera() const       { return r_camera; }
	inline ACollider * collider() const  { return r_collider; }
	inline Body * body() const           { return r_body; }

	// Returns the scene the entity belongs to.
	Scene & scene() const;

	// Sends a message to the components of the entity
	void sendMessage(const std::string & msg);

	void onCollisionEnter(const CollisionInfo & info);
	void onCollisionExit(const CollisionInfo & info);

	//--------------------------------------
	// Rendering & filtering
	//--------------------------------------

	// Draws the entity. Has no effect if it is inactive or without a renderer.
//	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	inline void setLayerMask(u32 mask) { m_layerMask = mask; }
	inline u32 layerMask() const { return m_layerMask; }

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

//	void flushComponents();

	//--------------------------------------
	// Attributes
	//--------------------------------------

	std::vector<AComponent*>  m_components;

	// References to engine components for quick lookup
	ARenderer *               r_renderer;
	ACollider *               r_collider;
	Body *                    r_body;
	Camera *                  r_camera;
	AAnimator *               r_animator;

	sf::Transformable         m_transform;
	u8                        m_flags;
	u32                       m_layerMask;

	Scene *                   r_scene;

};

} // namespace zn

#endif // HEADER_ZN_ENTITY_HPP_INCLUDED


