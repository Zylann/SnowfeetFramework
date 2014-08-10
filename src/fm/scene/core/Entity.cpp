#include <sstream>

#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/Component.hpp>
#include <fm/scene/graphics/Renderer.hpp>
#include <fm/scene/physics/Collider.hpp>
#include <fm/scene/physics/Body.hpp>
#include <fm/scene/core/Scene.hpp>
#include <fm/util/typecheck.hpp>
#include <fm/util/Log.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

//------------------------------------------------------------------------------
Entity::Entity() :
	m_layerIndex(0), // default layer
	r_scene(nullptr),
	m_tags(0)
{
#ifdef ZN_DEBUG
	setName("_entity_");
#endif

	m_flags.set(EF_ACTIVE);
}

//------------------------------------------------------------------------------
Entity::~Entity()
{
	// Clear components
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component * component = (*it);
		component->onDestroy();
		delete component;
	}

	// Clear tags
	for(u8 tagIndex = 0; tagIndex < TagManager::MAX_TAGS; ++tagIndex)
	{
		if(hasTag(tagIndex))
		{
			r_scene->tagManager.onEntityUntagged(this, tagIndex);
		}
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
	m_flags.set(EF_DESTROY_LATE, true);
	// Note: when an entity is marked as DESTROY_LATE,
	// its children will be destroyed too. See Scene.cpp for implementation details.
}

//------------------------------------------------------------------------------
bool Entity::active() const
{
	return m_flags[EF_ACTIVE];
}

//------------------------------------------------------------------------------
bool Entity::activeInHierarchy() const
{
	if(active())
	{
		Transform * t = transform();
		if(t && t->parent() != nullptr)
		{
			return active() && t->parent()->entity().activeInHierarchy();
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
	m_flags.set(EF_ACTIVE, active);
}

//------------------------------------------------------------------------------
void Entity::setCrossScene(bool crossScene)
{
	m_flags.set(EF_CROSS_SCENE, crossScene);
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
		log.err() << "Entity::setLayerByIndex: invalid index (" << layerIndex << ')' << log.endl();
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
Renderer * Entity::renderer() const    { return getComponent<Renderer>(); }
Animator * Entity::animator() const    { return getComponent<Animator>(); }
Collider * Entity::collider() const    { return getComponent<Collider>(); }
Body * Entity::body() const            { return getComponent<Body>(); }
AudioEmitter * Entity::audio() const   { return getComponent<AudioEmitter>(); }
Transform * Entity::transform() const  { return getComponent<Transform>(); }

//------------------------------------------------------------------------------
Component * Entity::getComponent(const ObjectType & cmpType, bool includeInheritance) const
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component * cmp = (*it);
		if(cmp->objectType().is(cmpType, includeInheritance))
		{
			return cmp;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
Component * Entity::addComponent(Component * newCmp)
{
	assert(newCmp != nullptr);

	m_components.push_back(newCmp);

	newCmp->onAdd(this);

	newCmp->onCreate();

	return newCmp;
}

//------------------------------------------------------------------------------
void Entity::removeComponent(Component * cmp)
{
	assert(cmp != nullptr);

	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		if((*it) == cmp)
		{
			cmp->onDestroy();
			m_components.erase(it);
			delete cmp;
			return;
		}
	}

	#ifdef ZN_DEBUG
	log.warn() << "Entity::removeComponent: not found " << cmp->objectType().toString() << log.endl();
	#endif
}

//------------------------------------------------------------------------------
void Entity::sendMessage(const std::string & msg)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->onMessage(msg);
	}
}

//------------------------------------------------------------------------------
void Entity::onCollisionEnter(const CollisionInfo & info)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->onCollisionEnter(info);
	}
}

//------------------------------------------------------------------------------
void Entity::onCollisionExit(const CollisionInfo & info)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->onCollisionExit(info);
	}
}

//------------------------------------------------------------------------------
void Entity::serialize(JsonBox::Value & o)
{
	// Meta

	o["id"]       = (s32)m_id;
	o["flags"]    = (u8)(m_flags.to_ulong());
	o["layer"]    = (s32)m_layerIndex; // TODO fix JsonBox so it accepts unsigned integers
	o["name"]     = name();
	o["tags"]     = (s32)m_tags;

	// Components

	JsonBox::Array componentListData;
	componentListData.resize(m_components.size());
	u32 i = 0;
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		JsonBox::Value & componentData = componentListData[i];
		Component::serialize(*it, componentData);
		++i;
	}
	o["components"] = componentListData;
}

//------------------------------------------------------------------------------
void Entity::unserialize(JsonBox::Value & o)
{
	// Meta

	m_id = o["id"].getInt();
	m_flags = std::bitset<8>((u8)(o["flags"].getInt()));
	m_layerIndex = o["layer"].getInt();
	m_tags = o["tags"].getInt();
	setName(o["name"].getString());

	// Components

	JsonBox::Value & componentListData = o["components"];
	u32 n = componentListData.getArray().size();
	for(u32 i = 0; i < n; ++i)
	{
		Component * component = Component::unserialize(componentListData[i]);

		if(!checkComponentAddition(component->objectType(), "Entity::unserialize"))
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
		(*it)->postUnserialize();
	}
}

//------------------------------------------------------------------------------
bool Entity::checkComponentAddition(const ObjectType & ct, const std::string & context)
{
	// Check component derivation
	if(!ct.is(zn::Component::sObjectType()))
	{
		log.err() << context << ": "
			"Cannot add a non-component object (" << ct.toString() << ")" << log.endl();
		return false;
	}

	// Check component type ID
	if(ct.ID == 0)
	{
		log.err() << context << ": "
			"The native component " << ct.name << " has no ID. "
			"It must be registered on application start." << log.endl();
		log.more() << "on entity \"" << m_name << '"' << log.endl();

		return false;
	}

	/*
	// Check duplicates
	auto componentIt = m_components.find(ct.ID);
	if(componentIt != m_components.end())
	{
		log.err() << context << ": "
			"cannot add two components of the same type ! " << log.endl();
		log.more() << ct.toString() << log.endl();
		log.more() << "on entity \"" << m_name << '"' << log.endl();

		return false;
	}

	// If the component is not a behaviour, test its unicity within the entity
	if(!ct.is(Behaviour::sObjectType()))
	{
		// Search for another component of the same group
		Component * duplicate = getComponent(ct);

		// If another has been found
		if(duplicate != nullptr)
		{
			log.err() << context << ": "
				"cannot add component, only one of its group is allowed !" << log.endl();
			log.more() << "adding: " << ct.toString() << log.endl();
			log.more() << "the entity already has: " << duplicate->objectType().toString() << log.endl();
			log.more() << "on entity \"" << m_name << '"' << log.endl();

			return false;
		}
	}
	*/

	return true;
}

//------------------------------------------------------------------------------
void Entity::addTag(u8 tagIndex)
{
	m_tags |= 1 << tagIndex;
	r_scene->tagManager.onEntityTagged(this, tagIndex);
}

//------------------------------------------------------------------------------
void Entity::removeTag(u8 tagIndex)
{
	m_tags &= ~(1 << tagIndex);
	r_scene->tagManager.onEntityUntagged(this, tagIndex);
}

//------------------------------------------------------------------------------
bool Entity::hasTag(u8 tagIndex) const
{
	return m_tags & (1 << tagIndex);
}

//------------------------------------------------------------------------------
void Entity::addTag(const std::string & tagName)
{
	s8 i = r_scene->tagManager.indexFromName(tagName);
	assert(i >= 0);
	addTag(i);
}

//------------------------------------------------------------------------------
void Entity::removeTag(const std::string & tagName)
{
	s8 i = r_scene->tagManager.indexFromName(tagName);
	assert(i >= 0);
	removeTag(i);
}

//------------------------------------------------------------------------------
bool Entity::hasTag(const std::string & tagName) const
{
	s8 i = r_scene->tagManager.indexFromName(tagName);
	if(i >= 0)
	{
		return hasTag(i);
	}
	else
	{
		return false;
	}
}

} // namespace zn





