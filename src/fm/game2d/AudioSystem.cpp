#include <cassert>
#include "AudioSystem.hpp"
#include "Entity.hpp"
#include "../asset/AssetBank.hpp"

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
		setListenerPosition(r_listenerEntity->position());
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

	music.update(delta);
}

//------------------------------------------------------------------------------
sf::Sound * AudioSystem::requestSource(AudioEmitter & newOwner)
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

//------------------------------------------------------------------------------
void MusicSystem::play(std::string name, bool loop)
{
	sf::Music * music = AssetBank::current()->soundStreams.get(name);
	assert(music != nullptr);

	if(r_currentMusic != nullptr)
	{
#ifdef ZN_DEBUG
		if(r_currentMusic == music)
		{
			std::cout << "D: MusicSystem::play: already playing \"" << name << "\", restart from 0" << std::endl;
		}
#endif
		r_currentMusic->stop();
	}

	r_currentMusic = music;
	r_currentMusic->play();
	r_currentMusic->setLoop(loop);
}

//------------------------------------------------------------------------------
void MusicSystem::stop()
{
	if(r_currentMusic != nullptr)
	{
		r_currentMusic->stop();
		r_currentMusic = nullptr;
	}
}

//------------------------------------------------------------------------------
void MusicSystem::setVolume(f32 volume)
{
	m_volume = volume;

	if(r_currentMusic != nullptr)
	{
		r_currentMusic->setVolume(volume);
	}
}

//------------------------------------------------------------------------------
void MusicSystem::pause()
{
	if(r_currentMusic != nullptr)
	{
		r_currentMusic->pause();
	}
}

//------------------------------------------------------------------------------
void MusicSystem::resume()
{
	if(r_currentMusic != nullptr && r_currentMusic->getStatus() == sf::SoundSource::Paused)
	{
		r_currentMusic->play();
	}
}

//------------------------------------------------------------------------------
void MusicSystem::update(sf::Time delta)
{
	if(r_currentMusic->getStatus() == sf::SoundSource::Status::Stopped)
	{
		r_currentMusic = nullptr;
	}

	// TODO MusicSystem: handle cross-fade
}

} // namespace zn



