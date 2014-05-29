#include <fm/scene/core/ComponentFactory.hpp>

// For engine registration
#include <fm/util/macros.hpp>
#include <fm/scene/animation/SpriteAnimator.hpp>
#include <fm/scene/audio/AudioEmitter.hpp>
#include <fm/scene/graphics/Camera.hpp>
#include <fm/scene/graphics/MapRenderer.hpp>
#include <fm/scene/graphics/ParticleSystem.hpp>
#include <fm/scene/graphics/SpriteRenderer.hpp>
#include <fm/scene/graphics/TextRenderer.hpp>
#include <fm/scene/graphics/OverlayRenderer.hpp>
#include <fm/scene/physics/BoxCollider.hpp>
#include <fm/scene/physics/MapCollider.hpp>

namespace zn
{

using namespace std;

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
void ComponentFactory::registerEngineComponents()
{
	ZN_CALL_ONCE;

	ComponentFactory & f = ComponentFactory::get();

	// Animation

	f.registerType<zn::SpriteAnimator>();

	// Audio

	f.registerType<zn::AudioEmitter>();

	// Graphics

	f.registerType<zn::Camera>();
	f.registerType<zn::MapRenderer>();
	f.registerType<zn::ParticleSystem>();
	f.registerType<zn::SpriteRenderer>();
	f.registerType<zn::TextRenderer>();
	f.registerType<zn::OverlayRenderer>();

	// Physics

	f.registerType<zn::BoxCollider>();
	f.registerType<zn::MapCollider>();

}

} // namespace zn

