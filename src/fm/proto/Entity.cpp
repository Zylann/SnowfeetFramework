#include <iostream>

#include <fm/proto/Entity.hpp>
#include <fm/proto/Component.hpp>
#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/physics/Collider.hpp>
#include <fm/proto/physics/Body.hpp>
#include <fm/proto/Scene.hpp>
#include <fm/util/typecheck.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

Entity::Entity() :
	transform(*this),
	r_renderer(nullptr),
	r_collider(nullptr),
	r_body(nullptr),
	r_camera(nullptr),
	r_animator(nullptr),
	m_flags(ACTIVE),
	m_layerMask(1), // default mask
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
void Entity::destroyLater()
{
	setFlag(DESTROY_LATE, true);
	// Propagate to children
	for(auto it = transform.begin(); it != transform.end(); ++it)
	{
		it->entity().destroyLater();
	}
}

//------------------------------------------------------------------------------
bool Entity::active() const
{
	return m_flags & ACTIVE;
}

//------------------------------------------------------------------------------
bool Entity::activeInHierarchy() const
{
	if(active())
	{
		if(transform.parent() != nullptr)
		{
			return active() && transform.parent()->entity().activeInHierarchy();
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------------
void Entity::setActive(bool active)
{
	setFlag(ACTIVE, active);
}

//------------------------------------------------------------------------------
void Entity::setCrossScene(bool crossScene)
{
	setFlag(CROSS_SCENE, crossScene);
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
		AComponent * duplicate = nullptr;
		for(u32 i = 0; i < m_components.size(); ++i)
		{
			if(m_components[i]->componentType().group == ct.group)
			{
				duplicate = m_components[i];
				break;
			}
		}
		if(duplicate != nullptr)
		{
			std::cout << "E: Entity::addComponent: duplicate of same group !" << std::endl;

			std::cout << "E: | adding: ";
			newCmp->componentType().print(std::cout);
			std::cout << std::endl;

			std::cout << "E: | duplicate is: ";
			duplicate->componentType().print(std::cout);
			std::cout << std::endl;
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
void Entity::serialize(JsonBox::Value & o)
{
	// Transform

	JsonBox::Value & transformData = o["transform"];
	transform.serialize(transformData);

	// Meta

	o["id"]         = (s32)m_id;
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
	transform.unserialize(transformData);

	// Meta

	m_id = o["id"].getInt();
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
void Entity::postUnserialize(JsonBox::Value & o)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->postUnserialize();
	}
}

} // namespace zn





