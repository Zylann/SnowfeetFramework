#ifndef HEADER_ZN_COMPONENT_HPP_INCLUDED
#define HEADER_ZN_COMPONENT_HPP_INCLUDED

#include <string>

#include <JsonBox.h>
#include <fm/types.hpp>
#include <fm/proto/physics/CollisionInfo.hpp>
#include <fm/proto/core/ComponentType.hpp>
#include <fm/sfml/InputListener.hpp>

namespace zn
{

class Entity;

/// \brief internal flags that can be set on a component (bitmasks)
enum ComponentFlags
{
	CF_ENABLED         = 1,
	CF_FIRST_UPDATE    = 1 << 1,
	CF_INPUT_LISTENER  = 1 << 2
};

/// \brief A piece that composes an entity.
/// Components are a core element of the engine.
/// Some will implement functionnality, others just wrap existing objects
/// to offer a component-friendly interface and engine-specific stuff.
/// Note: components that derive directly from AComponent are often specific to the engine.
/// if you want to implement gameplay, please derive from Behaviour instead.
/// For serialization and execution flow reasons, components must define a default
/// constructor and avoid to rely on a constructor with parameters.
class ZN_API AComponent : public IInputListener
{
public:

	virtual ~AComponent();

	/// \brief Returns the entity the component belongs to.
	/// \return the entity
	Entity & entity() const;

	/// \brief Returns the meta-class of the component
	/// \return meta-class
	virtual const ComponentType & componentType() const = 0;

	/// \brief Called immediately after the component to be added to an entity.
	/// This method is responsible for referencing the entity.
	virtual void onAdd(Entity * e);

	/// \brief Called to initialize the component after being added to an entity.
	virtual void onCreate();

	/// \brief Called just before the first update of the component.
	virtual void onStart();

	/// \brief Called just before the component to be destroyed.
	virtual void onDestroy();

	/// \brief Called when the owning entity receives a generic message.
	/// \note this function is very basic, it may change when an event system
	/// will be implemented.
	/// \param msg: contents of the message, usually just an identifier
	virtual void onMessage(const std::string & msg) {}

	/// \brief Enables or disables the component.
	/// \param enable: wether the component should be enabled or not.
	virtual void setEnabled(bool enable);

	/// \brief Tells if the component is enabled.
	inline bool enabled() const { return m_flags & CF_ENABLED; }

	/// \brief Enables the onEvent() callback for input events
	void receiveInput(bool enableInput);

	/// \bief Tells if the component receives input events
	inline bool receivesInput() const { return m_flags & CF_INPUT_LISTENER; }

	//----------
	// TODO make callbacks below optional by using an event system?

	/// \brief Called on each frame to update realtime behaviour.
	/// this function is not aimed at being override, use onUpdate() instead.
	void update();

	// TODO intergate a physics engine?

	/// \brief Called when the entity starts colliding with something
	/// \param info: infos about the collision such as the hit point, normal etc.
	/// \warning this function is not currently called by the engine, because
	/// it has no integrated physics engine yet.
	virtual void onCollisionEnter(const CollisionInfo & info) {}

	/// \brief Called when the entity exits a collision
	/// \param info: infos about the collision such as the hit point, normal etc.
	/// \warning this function is not currently called by the engine, because
	/// it has no integrated physics engine yet.
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

	/// \brief Saves the component as JSON data.
	/// Automatically adds type information from componentType().
	/// \param component: component to serialize
	/// \param o: output JSON data tree
	static void serialize(AComponent * component, JsonBox::Value & o);

	/// \brief Creates a component from its JSON representation
	/// Retrieves type information and use ComponentType factory to instantiate the component.
	/// \param o: input JSON data tree
	/// \return newly created component, or nullptr if something went wrong
	static AComponent * unserialize(JsonBox::Value & o);

	/// \brief Saves component's properties to JSON (no need to save the class ID).
	/// When subclassing, you need to call superclass' version.
	/// \param o: output JSON data tree
	virtual void serializeData(JsonBox::Value & o);

	/// \brief Initializes the component from JSON data
	/// When subclassing, you need to call superclass' version.
	/// \param o: input JSON data tree
	virtual void unserializeData(JsonBox::Value & o);

	/// \brief Called after unserialization of all components in the entity.
	/// Use this function to relink references, check integrity etc.
	/// \note this function may change a bit in the future to be more like
	/// Entity's version.
	virtual void postUnserialize() {}

protected:

	/// \brief Called on each frame to update realtime behaviour.
	virtual void onUpdate();

	AComponent();

private:

	/// \brief reference to the entity that owns the component.
	Entity * r_owner;

	/// \brief various flags about the state of the component.
	/// \see ComponentFlags.
	u8 m_flags;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENT_HPP_INCLUDED



