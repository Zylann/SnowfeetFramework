#include <fm/scene/core/ComponentFactory.hpp>

// For engine registration
#include <fm/util/macros.hpp>
#include <fm/scene/animation.hpp>
#include <fm/scene/audio.hpp>
#include <fm/scene/graphics.hpp>
#include <fm/scene/physics.hpp>
#include <fm/scene/behaviour.hpp>

namespace zn
{

using namespace std;

//------------------------------------------------------------------------------
Component * ComponentFactory::instantiate(const string className)
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
ComponentType * ComponentFactory::getType(const std::string & typeName)
{
	auto it = m_nameToID.find(typeName);
	if(it != m_nameToID.end())
	{
		return m_registeredTypes[it->second];
	}
	else
	{
//		#ifdef ZN_DEBUG
//		log.warn() << "ComponentFactory::getType: "
//			"type not found '" << typeName << "'" << log.endl();
//		#endif
		return nullptr;
	}
}

//------------------------------------------------------------------------------
void ComponentFactory::registerEngineComponents()
{
	ZN_CALL_ONCE;

	ComponentFactory & f = ComponentFactory::get();

	// Animation

	f.registerType<zn::Animator>();
	f.registerType<zn::SpriteAnimator>();

	// Audio

	f.registerType<zn::AudioEmitter>();

	// Graphics

	f.registerType<zn::Camera>();
	f.registerType<zn::Renderer>();
	f.registerType<zn::MapRenderer>();
	f.registerType<zn::ParticleSystem>();
	f.registerType<zn::SpriteRenderer>();
	f.registerType<zn::TextRenderer>();
	f.registerType<zn::OverlayRenderer>();

	// Physics

	f.registerType<zn::Collider>();
	f.registerType<zn::BoxCollider>();
	f.registerType<zn::MapCollider>();
	f.registerType<zn::Body>();

	// Behaviours

	f.registerType<zn::Behaviour>();

}

} // namespace zn

