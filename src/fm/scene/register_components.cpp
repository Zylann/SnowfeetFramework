#include <fm/scene/register_components.hpp>
#include <fm/reflect/ObjectTypeDatabase.hpp>
#include <fm/util/macros.hpp>

#include <fm/scene/animation.hpp>
#include <fm/scene/audio.hpp>
#include <fm/scene/graphics.hpp>
#include <fm/scene/physics.hpp>
#include <fm/scene/behaviour.hpp>
#include <fm/scene/core/Transform.hpp>

namespace zn
{

void registerEngineComponentTypes()
{
	ZN_CALL_ONCE;

	ObjectTypeDatabase & odb = ObjectTypeDatabase::get();

	// Base

	odb.registerType<zn::Component>();
	odb.registerType<zn::Transform>();

	// Animation

	odb.registerType<zn::Animator>();
	odb.registerType<zn::SpriteAnimator>();

	// Audio

	odb.registerType<zn::AudioEmitter>();

	// Graphics

	odb.registerType<zn::Camera>();
	odb.registerType<zn::Renderer>();
	odb.registerType<zn::MapRenderer>();
	odb.registerType<zn::ParticleSystem>();
	odb.registerType<zn::SpriteRenderer>();
	odb.registerType<zn::TextRenderer>();
	odb.registerType<zn::OverlayRenderer>();

	// Physics

	odb.registerType<zn::Collider>();
	odb.registerType<zn::BoxCollider>();
	odb.registerType<zn::MapCollider>();
	odb.registerType<zn::Body>();

	// Behaviours

	odb.registerType<zn::Behaviour>();
}

} // namespace zn



