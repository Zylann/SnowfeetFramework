/*
ComponentType.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <cassert>
#include <unordered_map>

#include "ComponentType.hpp"
#include "../util/macros.hpp"

#include "animation/SpriteAnimator.hpp"
#include "audio/AudioEmitter.hpp"
#include "graphics/Camera.hpp"
#include "graphics/MapRenderer.hpp"
#include "graphics/ParticleSystem.hpp"
#include "graphics/SpriteRenderer.hpp"
#include "graphics/TextRenderer.hpp"
#include "physics/BoxCollider.hpp"
#include "physics/MapCollider.hpp"

using namespace std;

namespace zn
{

unordered_map<string, function<AComponent*()>> s_factories;

//------------------------------------------------------------------------------
// static
void ComponentType::registerType(const string className, function<AComponent*()> factory)
{
#ifdef ZN_DEBUG
	if(s_factories.find(className) != s_factories.end())
	{
		cout << "W: registered the same component type twice ! (" << className << ")" << endl;
	}
#endif
	s_factories[className] = factory;
}

//------------------------------------------------------------------------------
// static
AComponent * ComponentType::instantiate(const string className)
{
	auto it = s_factories.find(className);
#ifdef ZN_DEBUG
	if(it == s_factories.end())
	{
		cout << "E: cannot instantiate " << className << ", factory not found" << endl;
	}
#endif
	assert(it != s_factories.end());
	return it->second();
}

//------------------------------------------------------------------------------
// This macro is a shortcut that minimizes code redundancy to reduce mistype errors
#define ZN_REGISTER_COMPONENT(__className)                                     \
	ComponentType::registerType(#__className, __className::instantiate)

void ComponentType::registerEngineComponents()
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



