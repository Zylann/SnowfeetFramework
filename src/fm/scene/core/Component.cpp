#include <iostream>
#include <cassert>

#include <fm/config.hpp>
#include <fm/scene/core/Component.hpp>
#include <fm/scene/core/ComponentFactory.hpp>
#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/Scene.hpp>

using namespace std;

namespace zn
{

//------------------------------------------------------------------------------
AComponent::AComponent() :
	r_owner(nullptr)
{
	m_flags.set(CF_ENABLED);
	m_flags.set(CF_FIRST_UPDATE);
}

//------------------------------------------------------------------------------
AComponent::~AComponent()
{
	if(receivesInput())
	{
		// Remove the event callback
		receiveInput(false);
	}
}

//------------------------------------------------------------------------------
void AComponent::setEnabled(bool enable)
{
	m_flags.set(CF_ENABLED, enable);
}

//------------------------------------------------------------------------------
void AComponent::receiveInput(bool enableInput)
{
	if(enableInput && !receivesInput())
	{
		m_flags.set(CF_INPUT_LISTENER, true);
		entity().scene().guiManager.addListener(this);
	}
	else if(receivesInput())
	{
		m_flags.set(CF_INPUT_LISTENER, false);
		entity().scene().guiManager.removeListener(this);
	}
}

//------------------------------------------------------------------------------
void AComponent::onAdd(Entity * e)
{
	r_owner = e;
	m_flags.set(CF_FIRST_UPDATE);
}

//------------------------------------------------------------------------------
void AComponent::onCreate()
{
}

//------------------------------------------------------------------------------
void AComponent::onStart()
{
}

//------------------------------------------------------------------------------
void AComponent::update()
{
	if(m_flags.test(CF_FIRST_UPDATE))
	{
		onStart();
		onUpdate();
		m_flags.set(CF_FIRST_UPDATE, false);
	}
	else
	{
		onUpdate();
	}
}

//------------------------------------------------------------------------------
void AComponent::onUpdate()
{
}

//------------------------------------------------------------------------------
void AComponent::onDestroy()
{
}

//------------------------------------------------------------------------------
Entity & AComponent::entity() const
{
#ifdef ZN_DEBUG
	if(r_owner == nullptr)
		cout << "E: AComponent::entity: _owner is null !" << endl;
#endif
	return *r_owner;
}

//------------------------------------------------------------------------------
// Serialization (member)

void AComponent::serializeData(JsonBox::Value & o)
{
	o["flags"] = (u8)(m_flags.to_ulong());
}

void AComponent::unserializeData(JsonBox::Value & o)
{
	m_flags = std::bitset<8>((u8)(o["flags"].getInt()));
}

//------------------------------------------------------------------------------
// Serialization (static)

// TODO use a PropertyTree instead of directly JsonBox::Object?
// It would solve the problem about which serialization format to choose...

void AComponent::serialize(AComponent * component, JsonBox::Value & o)
{
	// Serialize type
	o["type"] = JsonBox::Value(component->componentType().name);

	// Serialize properties
	JsonBox::Value & properties = o["data"];
	component->serializeData(properties);
}

//------------------------------------------------------------------------------
AComponent * AComponent::unserialize(JsonBox::Value & o)
{
	// Get type of the component
	std::string type = o["type"].getString();

	// Instantiate it
	AComponent * component = ComponentFactory::get().instantiate(type);

	// Deserialize properties
	assert(component != nullptr);
	component->unserializeData(o["data"]);

	return component;
}

} // namespace zn


