#include <iostream>
#include <cassert>
#include "../config.hpp"
#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

namespace zn
{

AComponent::AComponent() :
	r_owner(nullptr),
	m_flags(0)
{
	setName("_component_");
}

AComponent::~AComponent()
{
}

void AComponent::onAdd(Entity * e)
{
	r_owner = e;
}

void AComponent::init()
{
}

void AComponent::update()
{
}

void AComponent::onDestroy()
{
}

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
	o["name"] = name();
	o["flags"] = m_flags;
}

void AComponent::unserializeData(JsonBox::Value & o)
{
	setName(o["name"].getString());
	m_flags = o["flags"].getInt();
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

AComponent * AComponent::unserialize(JsonBox::Value & o)
{
	// Get type of the component
	std::string type = o["type"].getString();

	// Instantiate it
	AComponent * component = ComponentType::instantiate(type);

	// Deserialize properties
	assert(component != nullptr);
	component->unserializeData(o["data"]);

	return component;
}

} // namespace zn


