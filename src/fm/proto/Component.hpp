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

	// Warning: serializing a component can't be done at any time with the same
	// meaning, so it depends what you want to do.
	// For instance, during execution, if a behaviour adds components to
	// the entity, if you serialize it and then reloads it,
	// the same behaviour may end up adding components a second time, and there
	// will be duplicates.
	// serialization may be called in a "scene edition" state.
	// Doing that in "play state" might be useful as a debug dump.

	// Saves the component as JSON data.
	// Automatically adds type information from componentType().
	static void serialize(AComponent * component, JsonBox::Value & o);

	// Creates a component from JSON data
	// Retrieves type information to instantiate the component.
	static AComponent * unserialize(JsonBox::Value & o);

	// Saves component's data to JSON (no need to save the type).
	// You need to call superclass' version.
	virtual void serializeData(JsonBox::Value & o);

	// Initializes the component from JSON data
	// You need to call superclass' version.
	virtual void unserializeData(JsonBox::Value & o);

	// Called after unserialization of all components in the entity.
	// Use this function to relink references, check integrity etc.
	virtual void postUnserialize() {}

protected:

	AComponent();

private:

	Entity *  r_owner; // Reference to the entity
	u8        m_flags;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENT_HPP_INCLUDED



