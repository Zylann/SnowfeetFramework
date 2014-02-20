#include <iostream>

#include <fm/proto/core/Entity.hpp>
#include <fm/proto/core/Component.hpp>
#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/physics/Collider.hpp>
#include <fm/proto/physics/Body.hpp>
#include <fm/proto/core/Scene.hpp>
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
	m_layerIndex(0), // default layer
	r_scene(nullptr)
{
	setName("_entity_");
}

Entity::~Entity()
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		AComponent * component = it->second;
		component->onDestroy();
		delete component;
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
	// Note: when an entity is marked as DESTROY_LATE,
	// its children will be destroyed too. See Scene.cpp for implementation details.
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
void Entity::setLayerByName(const std::string & layerName)
{
	const Layer * layerObj = r_scene->layers[layerName];
	m_layerIndex = layerObj->index;
}

//------------------------------------------------------------------------------
void Entity::setLayerByIndex(u32 layerIndex)
{
#ifdef ZN_DEBUG
	if(layerIndex >= LayerMap::COUNT)
	{
		std::cout << "E: Entity::setLayerByIndex: invalid index (" << layerIndex << ')' << std::endl;
		return;
	}
#endif
	m_layerIndex = layerIndex;
}

//------------------------------------------------------------------------------
const Layer & Entity::layer() const
{
	return r_scene->layers[m_layerIndex];
}

//------------------------------------------------------------------------------
AComponent * Entity::addComponent(AComponent * newCmp)
{
	assert(newCmp != nullptr);

	const ComponentType & ct = newCmp->componentType();
	m_components[ct.ID] = newCmp;

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
	assert(cmp != nullptr);

	auto it = m_components.find(cmp->componentType().ID);
	if(it != m_components.end())
	{
		m_components.erase(it);
	}
#ifdef ZN_DEBUG
	else
	{
		std::cout << "W: Entity::removeComponent: not found ";
		cmp->componentType().print(std::cout);
		std::cout << std::endl;
	}
#endif
}

//------------------------------------------------------------------------------
void Entity::sendMessage(const std::string & msg)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->onMessage(msg);
	}
}

//------------------------------------------------------------------------------
void Entity::onCollisionEnter(const CollisionInfo & info)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->onCollisionEnter(info);
	}
}

//------------------------------------------------------------------------------
void Entity::onCollisionExit(const CollisionInfo & info)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->onCollisionExit(info);
	}
}

//------------------------------------------------------------------------------
void Entity::serialize(JsonBox::Value & o)
{
	// Transform

	JsonBox::Value & transformData = o["transform"];
	transform.serialize(transformData);

	// Meta

	o["id"]       = (s32)m_id;
	o["flags"]    = m_flags;
	o["layer"]    = (s32)m_layerIndex; // TODO fix JsonBox so it accepts unsigned integers
	o["name"]     = name();

	// Components

	JsonBox::Array componentListData;
	componentListData.resize(m_components.size());
	u32 i = 0;
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		JsonBox::Value & componentData = componentListData[i];
		AComponent::serialize(it->second, componentData);
		++i;
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
	m_layerIndex = o["layer"].getInt();
	setName(o["name"].getString());

	// Components

	JsonBox::Value & componentListData = o["components"];
	u32 n = componentListData.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		AComponent * component = AComponent::unserialize(componentListData[i]);

		if(!checkComponentAddition(component->componentType(), "Entity::unserialize"))
		{
			delete component;
			continue;
		}

		addComponent(component);
	}
}

//------------------------------------------------------------------------------
void Entity::postUnserialize(JsonBox::Value & o)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->postUnserialize();
	}
}

//------------------------------------------------------------------------------
bool Entity::checkComponentAddition(const ComponentType & ct, const std::string & context)
{
	// Check component type ID
	if(ct.ID == 0)
	{
		std::cout << "E: " << context << ": "
			"The native component " << ct.name << " has no ID. "
			"It must be registered on application start." << std::endl;
		std::cout << "E: | on entity \"" << m_name << '"' << std::endl;

		return false;
	}

	// Check duplicates
	auto componentIt = m_components.find(ct.ID);
	if(componentIt != m_components.end())
	{
		// Found duplicate
		std::cout << "E: " << context << ": "
			"cannot add two components of the same type ! " << std::endl;
		std::cout << "E: | "; ct.print(std::cout); std::cout << std::endl;
		std::cout << "E: | on entity \"" << m_name << '"' << std::endl;

		return false;
	}

	// If the component is not a behaviour, test its group unicity
	if(ct.group != CG_BEHAVIOUR)
	{
		// Search for another component of the same group
		AComponent * duplicate = nullptr;
		for(auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			AComponent * c = it->second;
			if(c->componentType().group == ct.group)
			{
				duplicate = c;
				break;
			}
		}
		// If another has been found
		if(duplicate != nullptr)
		{
			std::cout << "E: " << context << ": "
				"cannot add component, only one of its group is allowed !" << std::endl;

			std::cout << "E: | adding: ";
			ct.print(std::cout);
			std::cout << std::endl;

			std::cout << "E: | the entity already has : ";
			duplicate->componentType().print(std::cout);
			std::cout << std::endl;

			std::cout << "E: | on entity \"" << m_name << '"' << std::endl;

			return false;
		}
	}

	return true;
}

} // namespace zn





