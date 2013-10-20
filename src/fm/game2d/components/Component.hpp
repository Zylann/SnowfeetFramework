#ifndef HEADER_ZN_COMPONENT_HPP_INCLUDED
#define HEADER_ZN_COMPONENT_HPP_INCLUDED

#include <string>
#include "../../types.hpp"
#include "../Object.hpp"
#include "../CollisionInfo.hpp"
#include "ComponentType.hpp"

namespace zn
{

class Entity;

// A piece that composes an entity.
// Note: components that derive directly from Component are specific to the engine.
// if you want to implement gameplay, please derive from Behaviour instead.
class AComponent : public Object
{
public:

	virtual ~AComponent();

	virtual void onAdd(Entity * e);
	virtual void init();
	virtual void update();
	virtual void onDestroy();

	Entity & entity() const;

	// Returns the meta-class of the component
	virtual const ComponentType & componentType() const = 0;

	// Called when the owning entity receives a generic message
	virtual void onMessage(const std::string & msg) {}

	virtual void onCollisionEnter(const CollisionInfo & info) {}
	virtual void onCollisionExit(const CollisionInfo & info) {}

protected:

	AComponent();

private:

	Entity *  r_owner;
	u8        m_flags;

};

} // namespace zn

#endif // HEADER_ZN_COMPONENT_HPP_INCLUDED



