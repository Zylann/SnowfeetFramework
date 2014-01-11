#include <cassert>
#include "AudioSystem.hpp"
#include "../Entity.hpp"
#include "../../asset/AssetBank.hpp"

namespace zn
{

AudioSystem::AudioSystem()
{
    setListenerPosition(sf::Vector2f(0,0));
    setListenerDepth(1);
    m_sources.resize(DEFAULT_SOURCES_COUNT);
}

AudioSystem::~AudioSystem()
{
}

//------------------------------------------------------------------------------
void AudioSystem::setListenerDepth(f32 z)
{
	sf::Vector3f pos = sf::Listener::getPosition();
	pos.z = z;
	sf::Listener::setPosition(pos);
}

//------------------------------------------------------------------------------
void AudioSystem::setListenerPosition(sf::Vector2f position)
{
	sf::Vector3f pos = sf::Listener::getPosition();
	sf::Listener::setPosition(position.x, position.y, pos.z);
}

//------------------------------------------------------------------------------
void AudioSystem::setListener(const Entity * e)
{
	r_listenerEntity = e;
}

//------------------------------------------------------------------------------
void AudioSystem::registerSource(AudioEmitter* source)
{
	assert(source != nullptr);
	m_emitters.insert(source);
}

//------------------------------------------------------------------------------
void AudioSystem::unregisterSource(AudioEmitter* source)
{
	m_emitters.erase(source);
}

//------------------------------------------------------------------------------
void AudioSystem::update(sf::Time delta)
{
	if(r_listenerEntity != nullptr)
	{
		setListenerPosition(r_listenerEntity->transform.position());
	}

	// Iterate over a copy to avoid concurrent modification
	auto updateList = m_emitters;

	for(auto it = updateList.begin(); it != updateList.end(); ++it)
	{
		AudioEmitter & cmp = **it;
		if(cmp.entity().active())
		{
			cmp.update();
		}
	}

//	music.update(delta);
}

//------------------------------------------------------------------------------
AudioSource * AudioSystem::requestSource(AudioEmitter & newOwner)
{
	std::vector<AttachedSource>::iterator it;
	for(it = m_sources.begin(); it != m_sources.end(); ++it)
	{
		AttachedSource & s = (*it);
		if(s.source.getStatus() == sf::SoundSource::Stopped)
		{
			// Detaches the source from its last AudioEmitter
			if(s.emitterRef)
			{
				s.emitterRef->detachSource(&s.source);
			}

			// Attaches the source to the new emitter
			s.emitterRef = &newOwner;

			// Returns the source
			return &s.source;
		}
	}
	return nullptr; // No source available
}

//------------------------------------------------------------------------------
void AudioSystem::detachEmitterSources(AudioEmitter & emitter)
{
	std::vector<AttachedSource>::iterator it;
	for(it = m_sources.begin(); it != m_sources.end(); ++it)
	{
		AttachedSource & s = *it;
		if(s.emitterRef == &emitter)
		{
			s.emitterRef->detachSource(&s.source);
			s.emitterRef = nullptr;
		}
	}
}

//------------------------------------------------------------------------------
void AudioSystem::setGlobalVolume(float volume)
{
	sf::Listener::setGlobalVolume(volume / 100.f);
}


} // namespace zn



