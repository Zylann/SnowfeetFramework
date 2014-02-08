#include <fm/proto/ComponentFactory.hpp>

// For engine registration
#include <fm/util/macros.hpp>
#include <fm/proto/animation/SpriteAnimator.hpp>
#include <fm/proto/audio/AudioEmitter.hpp>
#include <fm/proto/graphics/Camera.hpp>
#include <fm/proto/graphics/MapRenderer.hpp>
#include <fm/proto/graphics/ParticleSystem.hpp>
#include <fm/proto/graphics/SpriteRenderer.hpp>
#include <fm/proto/graphics/TextRenderer.hpp>
#include <fm/proto/physics/BoxCollider.hpp>
#include <fm/proto/physics/MapCollider.hpp>

namespace zn
{

using namespace std;

//------------------------------------------------------------------------------
void ComponentFactory::registerType(const string className, function<AComponent*()> factory)
{
#ifdef ZN_DEBUG
	if(m_factories.find(className) != m_factories.end())
	{
		cout << "W: registered the same component type twice ! (" << className << ")" << endl;
	}
#endif
	m_factories[className] = factory;
}

//------------------------------------------------------------------------------
AComponent * ComponentFactory::instantiate(const string className)
{
	auto it = m_factories.find(className);
#ifdef ZN_DEBUG
	if(it == m_factories.end())
	{
		cout << "E: cannot instantiate " << className << ", factory not found" << endl;
	}
#endif
	assert(it != m_factories.end());
	return it->second();
}

//------------------------------------------------------------------------------
// This macro is a shortcut that minimizes code redundancy to reduce mistype errors
#define ZN_REGISTER_COMPONENT(__className)                                     \
	ComponentFactory::get().registerType(#__className, __className::instantiate)

void ComponentFactory::registerEngineComponents()
{
	ZN_CALL_ONCE;

	ZN_REGISTER_COMPONENT( zn::SpriteAnimator );

	ZN_REGISTER_COMPONENT( zn::AudioEmitter );

	ZN_REGISTER_COMPONENT( zn::Camera );

	ZN_REGISTER_COMPONENT( zn::MapRenderer );
	ZN_REGISTER_COMPONENT( zn::ParticleSystem );
	ZN_REGISTER_COMPONENT( zn::SpriteRenderer );
	ZN_REGISTER_COMPONENT( zn::TextRenderer );

	ZN_REGISTER_COMPONENT( zn::BoxCollider );
	ZN_REGISTER_COMPONENT( zn::MapCollider );

}

} // namespace zn

