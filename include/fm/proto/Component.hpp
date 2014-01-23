#ifndef HEADER_ZN_COMPONENT_HPP_INCLUDED
#define HEADER_ZN_COMPONENT_HPP_INCLUDED

#include <string>

#include <JsonBox.h>
#include <fm/types.hpp>
#include <fm/proto/physics/CollisionInfo.hpp>
#include <fm/proto/ComponentType.hpp>

namespace zn
{

class Entity;

enum ComponentFlags
{
	CF_FIRST_UPDATE = 1
};

// A piece that composes an entity.
// Components are a core element of the engine.
// Some will implement functionnality, others just wrap existing objects
// to offer a component-friendly interface and engine-specific stuff.
// Note: components that derive directly from AComponent are often specific to the engine.
// if you want to implement gameplay, please derive from Behaviour instead.
// For serialization and execution flow reasons, components must define a default
// constructor and avoid to rely on a constructor with parameters.
class ZN_API AComponent
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

	// Called just before the first update of the component.
	virtual void onStart();

	// Called just before the component to be destroyed.
	virtual void onDestroy();

	// Called when the owning entity receives a generic message
	virtual void onMessage(const std::string & msg) {}

	//----------
	// TODO make callbacks below optional by using an event system?

	// Called on each frame to update realtime behaviour.
	void update();

	// Called when the entity starts colliding with something
	virtual void onCollisionEnter(const CollisionInfo & info) {}

	// Called when the entity exits a collision
	virtual void onCollisionExit(const CollisionInfo & info) {}

	// --------------------
	// Serialization
	// --------------------

	// Please note:
	//
	// serializing a component can't be done at any time with the same
	// meaning, so it depends what you want to do.
	// For instance, during execution, if a behaviour adds components to
	// the entity, if you serialize it and then reloads it,
	// the same behaviour will end up adding components a second time, and there
	// will be duplicates.
	// serialization may be called in a "scene edition" state.
	// Doing that in "play state" might be useful as a debug dump.

	// Saves the component as JSON data.
	// Automatically adds type information from componentType().
	static void serialize(AComponent * component, JsonBox::Value & o);

	// Creates a component from JSON data
	// Retrieves type information and use ComponentType factory to instantiate the component.
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

	// Called on each frame to update realtime behaviour.
	virtual void onUpdate();

	AComponent();

private:

	Entity *  r_owner; // Reference to the entity
	u8        m_flags;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENT_HPP_INCLUDED



