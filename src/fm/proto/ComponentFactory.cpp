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

	// Physics

	f.registerType<zn::BoxCollider>();
	f.registerType<zn::MapCollider>();

}

} // namespace zn

