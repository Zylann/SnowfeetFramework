#include <iostream>
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
// Serialization (static)

// TODO use a PropertyTree instead of directly JsonBox::Object?
// It would solve the problem about which serialization format to choose...

void AComponent::serialize(AComponent * component, JsonBox::Object & o)
{
	// TODO implement serialization
	cout << "E: Not implemented yet" << endl;
}

AComponent * AComponent::unserialize(const JsonBox::Object & o)
{
	// TODO implement unserialization
	cout << "E: Not implemented yet" << endl;
	return nullptr;
}

} // namespace zn


