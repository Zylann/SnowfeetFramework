#include "AudioEmitter.hpp"
#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../../sfml/sfml2_utils.hpp"
#include "../../asset/AssetBank.hpp"

namespace zn
{

void AudioEmitter::onAdd(Entity* e)
{
	AComponent::onAdd(e);
	entity().scene().audioSystem.registerSource(this);
}

//------------------------------------------------------------------------------
void AudioEmitter::onDestroy()
{
	AudioSystem & system = entity().scene().audioSystem;

	if(!m_sourceRefs.empty())
	{
		system.detachEmitterSources(*this);
	}

	system.unregisterSource(this);
}

//------------------------------------------------------------------------------
void AudioEmitter::update()
{
	sf::Vector2f pos = entity().position();
	std::unordered_set<AudioSource*>::iterator it;
	for(it = m_sourceRefs.begin(); it != m_sourceRefs.end(); it++)
	{
		(*it)->setPosition(pos.x, pos.y, 0);
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::setAttenuation(f32 attenuation)
{
	if(attenuation < 0.01f)
		attenuation = 0.01f;
	m_attenuation = attenuation;

	// Update current sounds
	for(auto it = m_sourceRefs.begin(); it != m_sourceRefs.end(); ++it)
	{
		(*it)->setAttenuation(m_attenuation);
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::setRadii(f32 minRadius, f32 maxRadius)
{
	if(minRadius < 0.01f)
		minRadius = 0.01f;
	if(maxRadius < minRadius)
		maxRadius = minRadius;

	// Update current sounds
	for(auto it = m_sourceRefs.begin(); it != m_sourceRefs.end(); ++it)
	{
		(*it)->setMinDistance(m_minRadius);
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::play(std::string soundName, f32 volume, f32 pitch, bool loop)
{
#ifdef ZN_DEBUG
	std::cout << "D: AudioEmitter: play " << soundName << std::endl;
#endif

	// TODO AudioEmitter: search for a streamed sound if no sound is found

	AudioSystem & system = entity().scene().audioSystem;
	sf::Vector2f position = entity().position();

	// If we are beyond the maximal radius, the sound is not played
	if(zn::distance(system.listenerPosition(), position) > m_maxRadius)
	{
		return;
	}

	// Get sound buffer
	const sf::SoundBuffer * soundBuffer = AssetBank::current()->sounds.get(soundName);

#ifdef ZN_DEBUG
	if(soundBuffer == nullptr)
	{
		std::cout << "E: AudioEmitter::play: no such sound (" << soundName << ")" << std::endl;
	}
#endif
	assert(soundBuffer != nullptr);

	AudioSource * sound = getFreeSource();
	// If the sound can be played
	if(sound)
	{
		// Configure and play it
		sound->setBuffer(*soundBuffer);
		sound->setVolume(volume*100.f);
		sound->setPitch(pitch);
		sound->setPosition(position.x, position.y, 0);
		sound->setLoop(loop);
		sound->setMinDistance(m_minRadius);
		sound->setAttenuation(m_attenuation);
		sound->setRelativeToListener(!m_spatialize);
		sound->play();
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::stop()
{
	for(auto it = m_sourceRefs.begin(); it != m_sourceRefs.end(); ++it)
	{
		(*it)->stop();
	}
	m_sourceRefs.clear();
}

//------------------------------------------------------------------------------
void AudioEmitter::setSpatialized(bool spatialize)
{
	m_spatialize = spatialize;

	for(auto it = m_sourceRefs.begin(); it != m_sourceRefs.end(); ++it)
	{
		(*it)->setRelativeToListener(!m_spatialize);
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::detachSource(AudioSource* s)
{
	s->stop();
	m_sourceRefs.erase(s);
}

//------------------------------------------------------------------------------
AudioSource * AudioEmitter::getFreeSource()
{
	// Search for a source already referenced
	std::unordered_set<AudioSource*>::iterator it;
	for(it = m_sourceRefs.begin(); it != m_sourceRefs.end(); it++)
	{
		if((*it)->getStatus() == sf::SoundSource::Stopped)
		{
			return *it;
		}
	}

	// Otherwise, get a new source from the audio system
	AudioSource * s = entity().scene().audioSystem.requestSource(*this);
	if(s != 0)
	{
		sf::Vector2f pos = entity().position();
		s->setPosition(pos.x, pos.y, 0);
		m_sourceRefs.insert(s);
	}

	return s; // Note: can be nullptr if not found
}

} // namespace zn




