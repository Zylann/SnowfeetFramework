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
	std::unordered_set<sf::Sound*>::iterator it;
	for(it = m_sourceRefs.begin(); it != m_sourceRefs.end(); it++)
	{
		(*it)->setPosition(pos.x, pos.y, 0);
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::play(std::string soundName, f32 volume, f32 pitch, bool loop)
{
#ifdef ZN_DEBUG
	std::cout << "D: AudioEmitter: play " << soundName << std::endl;
#endif

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

	sf::Sound * sound = getFreeSource();
	// If the sound can be played
	if(sound)
	{
		// Configure and play it
		sound->setBuffer(*soundBuffer);
		sound->setVolume(volume*100.f);
		sound->setPitch(pitch);
		sound->setPosition(position.x, position.y, 0);
		sound->setLoop(loop);
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
void AudioEmitter::detachSource(sf::Sound* s)
{
	s->stop();
	m_sourceRefs.erase(s);
}

//------------------------------------------------------------------------------
sf::Sound * AudioEmitter::getFreeSource()
{
	// Search for a source already referenced
	std::unordered_set<sf::Sound*>::iterator it;
	for(it = m_sourceRefs.begin(); it != m_sourceRefs.end(); it++)
	{
		if((*it)->getStatus() == sf::SoundSource::Stopped)
		{
			return *it;
		}
	}

	// Otherwise, get a new source from the audio system
	sf::Sound * s = entity().scene().audioSystem.requestSource(*this);
	if(s != 0)
	{
		sf::Vector2f pos = entity().position();
		s->setPosition(pos.x, pos.y, 0);
		m_sourceRefs.insert(s);
	}

	return s; // Note: can be nullptr if not found
}

} // namespace zn




