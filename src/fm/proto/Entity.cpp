#include <iostream>
#include "Entity.hpp"
#include "Component.hpp"
#include "graphics/Renderer.hpp"
#include "physics/Collider.hpp"
#include "physics/Body.hpp"
#include "Scene.hpp"
#include "../util/typecheck.hpp"
#include "../json/json_utils.hpp"

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

	m_components.push_back(newCmp);

	// Assign quick lookup pointer
	switch(ct.group)
	{
	case CG_RENDERER:       r_renderer     = checked_cast<ARenderer*>(newCmp); break;
	case CG_COLLIDER:       r_collider     = checked_cast<ACollider*>(newCmp); break;
	case CG_CAMERA:         r_camera       = checked_cast<Camera*>(newCmp); break;
	case CG_BODY:           r_body         = checked_cast<Body*>(newCmp); break;
	case CG_ANIMATOR:       r_animator     = checked_cast<AAnimator*>(newCmp); break;
	case CG_AUDIO_EMITTER:  r_audioEmitter = checked_cast<AudioEmitter*>(newCmp); break;
	default: break;
	}

	newCmp->onAdd(this);

	newCmp->init();

	return newCmp;
}

//------------------------------------------------------------------------------
void Entity::removeComponent(AComponent * cmp)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		if(*it == cmp)
		{
			delete cmp;
			m_components.erase(it);
			return;
		}
	}

#ifdef ZN_DEBUG
	std::cout << "E: Entity::removeComponnet: not found" << std::endl;
#endif
}

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

//------------------------------------------------------------------------------
//	sf::Transformable         m_transform;
//	u8                        m_flags;
//	u32                       m_layerMask;

void Entity::serialize(JsonBox::Value & o)
{
	// Transform

	JsonBox::Value & transformData = o["transform"];
	zn::serialize(transformData["position"], m_transform.getPosition());
	zn::serialize(transformData["scale"], m_transform.getScale());
	transformData["rotation"] = m_transform.getRotation();

	// Meta

	o["flags"]      = m_flags;
	o["layerMask"]  = (s32)m_layerMask; // TODO fix JsonBox so it accepts unsigned integers
	o["name"]       = name();

	// Components

	JsonBox::Array componentListData;
	componentListData.resize(m_components.size());
	for(u32 i = 0; i < m_components.size(); ++i)
	{
		JsonBox::Value & componentData = componentListData[i];
		AComponent::serialize(m_components[i], componentData);
	}
	o["components"] = componentListData;
}

//------------------------------------------------------------------------------
void Entity::unserialize(JsonBox::Value & o)
{
	// Transform

	JsonBox::Value transformData = o["transform"];

	sf::Vector2f v;

	zn::unserialize(transformData["position"], v);  m_transform.setPosition(v);
	zn::unserialize(transformData["scale"], v);     m_transform.setScale(v);
	m_transform.setRotation(transformData["rotation"].getDouble());

	// Meta

	m_flags = o["flags"].getInt();
	m_layerMask = o["layerMask"].getInt();
	setName(o["name"].getString());

	// Components

	JsonBox::Value & componentListData = o["components"];
	u32 n = componentListData.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		AComponent * component = AComponent::unserialize(componentListData[i]);
		addComponent(component);
	}
}

//------------------------------------------------------------------------------
void Entity::postUnserialize()
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->postUnserialize();
	}
}

} // namespace zn





