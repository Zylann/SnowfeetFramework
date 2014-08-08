#include <fm/scene/audio/AudioEmitter.hpp>
#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/Scene.hpp>
#include <fm/sfml/sfml2_utils.hpp>
#include <fm/asset/AssetBank.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::AudioEmitter)

//------------------------------------------------------------------------------
void AudioEmitter::onAdd(Entity* e)
{
	Component::onAdd(e);
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
void AudioEmitter::onUpdate()
{
	if(entity().transform())
	{
		sf::Vector2f pos = entity().transform()->position();
		setPosition(pos);
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::setPosition(const sf::Vector2f & newPosition)
{
	m_position.x = newPosition.x;
	m_position.y = newPosition.y;

	std::unordered_set<AudioSource*>::iterator it;
	for(it = m_sourceRefs.begin(); it != m_sourceRefs.end(); it++)
	{
		(*it)->setPosition(m_position);
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
bool AudioEmitter::canBeHeard()
{
	if(m_spatialize)
	{
		AudioSystem & system = entity().scene().audioSystem;

		// If we are beyond the maximal radius, the sound is not played
		return zn::distance(system.listenerPosition(), sf::Vector2f(m_position.x, m_position.y)) <= m_maxRadius;
	}
	else
	{
		// Not-spatialized sounds can always be heard
		return true;
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::playBuffer(std::string name, f32 volume, f32 pitch, bool loop)
{
#ifdef ZN_DEBUG
	std::cout << "D: AudioEmitter: playBuffer " << name << std::endl;
#endif

	if(!canBeHeard())
		return;

	// TODO AudioEmitter: search for a streamed sound if no sound buffer is found

	// Get sound buffer
	const sf::SoundBuffer * soundBuffer = AssetBank::current()->soundBuffers.get(name);

#ifdef ZN_DEBUG
	if(soundBuffer == nullptr)
	{
		std::cout << "E: AudioEmitter::play: no such sound (" << name << ")" << std::endl;
	}
#endif
	assert(soundBuffer != nullptr);

	AudioSource * source = getFreeSource();
	// If the sound can be played
	if(source)
	{
		if(entity().transform())
		{
			sf::Vector2f pos2d = entity().transform()->position();
			m_position.x = pos2d.x;
			m_position.y = pos2d.y;
		}

		// Configure and play it
		source->setBuffer(*soundBuffer);
		source->setVolume(volume*100.f);
		source->setPitch(pitch);
		source->setPosition(m_position);
		source->setLoop(loop);
		source->setMinDistance(m_minRadius);
		source->setAttenuation(m_attenuation);
		source->setRelativeToListener(!m_spatialize);
		source->play();
	}
}

//------------------------------------------------------------------------------
void AudioEmitter::playStream(std::string name, f32 volume, f32 pitch, bool loop)
{
#ifdef ZN_DEBUG
	std::cout << "D: AudioEmitter: playStream " << name << std::endl;
#endif

	if(!canBeHeard())
		return;

	// TODO AudioEmitter: search for a streamed sound if no sound buffer is found

	// Get sound buffer
	const FileRef * fileRef = AssetBank::current()->soundStreams.get(name);

#ifdef ZN_DEBUG
	if(fileRef == nullptr)
	{
		std::cout << "E: AudioEmitter::playStream: no such sound (" << name << ")" << std::endl;
	}
#endif
	assert(fileRef != nullptr);

	AudioSource * source = getFreeSource();
	// If the sound can be played
	if(source)
	{
		if(entity().transform())
		{
			sf::Vector2f pos2d = entity().transform()->position();
			m_position.x = pos2d.x;
			m_position.y = pos2d.y;
		}

		// Configure and play it
		source->setStreamFromFile(fileRef->filePath);
		source->setVolume(volume*100.f);
		source->setPitch(pitch);
		source->setPosition(m_position);
		source->setLoop(loop);
		source->setMinDistance(m_minRadius);
		source->setAttenuation(m_attenuation);
		source->setRelativeToListener(!m_spatialize);
		source->play();
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
		if(entity().transform())
		{
			sf::Vector2f pos2d = entity().transform()->position();
			m_position.x = pos2d.x;
			m_position.y = pos2d.y;
		}

		s->setPosition(m_position);
		m_sourceRefs.insert(s);
	}

	return s; // Note: can be nullptr if not found
}

//------------------------------------------------------------------------------
void AudioEmitter::serializeData(JsonBox::Value & o)
{
	Component::serializeData(o);

	o["spatialize"] = m_spatialize;
	o["minRadius"] = m_minRadius;
	o["maxRadius"] = m_maxRadius;
	o["attenuation"] = m_attenuation;
	zn::serialize(o["position"], m_position);
}

//------------------------------------------------------------------------------
void AudioEmitter::unserializeData(JsonBox::Value & o)
{
	Component::unserializeData(o);

	m_spatialize = o["spatialize"].getBoolean();
	m_minRadius = o["minRadius"].getDouble();
	m_maxRadius = o["maxRadius"].getDouble();
	m_attenuation = o["attenuation"].getDouble();
	zn::unserialize(o["position"], m_position);
}

//------------------------------------------------------------------------------
void AudioEmitter::postUnserialize()
{

}


} // namespace zn




