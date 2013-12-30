#ifndef HEADER_ZN_COMPONENT_HPP_INCLUDED
#define HEADER_ZN_COMPONENT_HPP_INCLUDED

#include <string>

#include "../types.hpp"
#include "../json/JsonBox.h"
#include "Object.hpp"
#include "physics/CollisionInfo.hpp"
#include "ComponentType.hpp"

namespace zn
{

class Entity;

// A piece that composes an entity.
// Note: components that derive directly from Component are specific to the engine.
// if you want to implement gameplay, please derive from Behaviour instead.
// For serialization and execution flow reasons, components must define a default
// constructor and avoid to rely on a constructor with parameters.
class AComponent : public Object
{
public:

	virtual ~AComponent();

	// Returns the entity the component belongs to.
	Entity & entity() const;

	// Returns the meta-class of the component
	virtual const ComponentType & componentType() const = 0;

	// Called immediately after the component to be added to an entity.
	// This method is responsible for referencing the entity.
	virtual void onAdd(Entity * e);

	// Called to initialize the component after being added to an entity.
	virtual void init();

	// Called just before the component to be destroyed.
	virtual void onDestroy();

	// Called when the owning entity receives a generic message
	virtual void onMessage(const std::string & msg) {}

	//----------
	// TODO make callbacks below optional by using an event system?

	// Called on each frame to update realtime behaviour.
	virtual void update();

	// Called when the entity starts colliding with something
	virtual void onCollisionEnter(const CollisionInfo & info) {}

	// Called when the entity exits a collision
	virtual void onCollisionExit(const CollisionInfo & info) {}

	// --------------------
	// Serialization
	// --------------------

	// Saves the component as JSON data
	static void serialize(AComponent * component, JsonBox::Object & o);

	// Creates a component from JSON data
	static AComponent * unserialize(const JsonBox::Object & o);

protected:

	AComponent();

private:

	Entity *  r_owner; // Reference to the entity
	u8        m_flags;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENT_HPP_INCLUDED



