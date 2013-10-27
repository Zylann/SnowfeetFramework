#include <iostream>
#include "../config.hpp"
#include "Component.hpp"
#include "Entity.hpp"

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
		std::cout << "E: AComponent::entity: _owner is null !" << std::endl;
#endif
	return *r_owner;
}

} // namespace zn


