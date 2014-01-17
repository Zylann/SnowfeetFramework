#include <cassert>
#include <fm/sfml/AudioSource.hpp>

#ifdef ZN_DEBUG
#include <iostream>
#endif

namespace zn
{

struct AudioSourceParameters
{
	void copyFrom(const sf::SoundSource & s)
	{
		pitch = s.getPitch();
		volume = s.getVolume();
		position = s.getPosition();
		relativeToListener = s.isRelativeToListener();
		minDistance = s.getMinDistance();
		attenuation = s.getAttenuation();
	}

	void applyTo(sf::SoundSource & s)
	{
		s.setPitch(pitch);
		s.setVolume(volume);
		s.setPosition(position);
		s.setRelativeToListener(relativeToListener);
		s.setMinDistance(minDistance);
		s.setAttenuation(attenuation);
	}

	f32           pitch = 1;
	f32           volume = 100;
	sf::Vector3f  position;
	bool          relativeToListener = false;
	f32           minDistance = 1;
	f32           attenuation = 0;
};


void AudioSource::setBuffer(const sf::SoundBuffer & buffer)
{
	if(m_sound == nullptr)
	{
		AudioSourceParameters parms;

		if(m_streamedSound != nullptr)
		{
			parms.copyFrom(*m_streamedSound);
			delete m_streamedSound;
			m_streamedSound = nullptr;
		}

		m_sound = new sf::Sound();
		parms.applyTo(*m_sound);
	}

	m_sound->setBuffer(buffer);

	m_streamed = false;
}

void AudioSource::setStreamFromFile(const std::string & filePath)
{
	if(m_streamedSound == nullptr)
	{
		AudioSourceParameters parms;

		if(m_sound != nullptr)
		{
			parms.copyFrom(*m_sound);
			delete m_sound;
			m_sound = nullptr;
		}

		m_streamedSound = new sf::Music();
		parms.applyTo(*m_streamedSound);
	}

	bool soundStreamWasLoaded = m_streamedSound->openFromFile(filePath);
#ifdef ZN_DEBUG
	if(!soundStreamWasLoaded)
	{
		std::cout << "E: AudioSource::setStreamFromFile: unable to open \"" << filePath << '"' << std::endl;
	}
#endif
	assert(soundStreamWasLoaded);

	m_streamed = true;
}

sf::Time AudioSource::getDuration() const
{
	if(m_streamed)
		return m_streamedSound->getDuration();
	else
		return m_sound->getBuffer()->getDuration();
}

void AudioSource::play()
{
	if(m_streamed)
		m_streamedSound->play();
	else
		m_sound->play();
}

void AudioSource::pause()
{
	if(m_streamed)
		m_streamedSound->pause();
	else
		m_sound->pause();
}

void AudioSource::stop()
{
	if(m_streamed)
		m_streamedSound->pause();
	else
		m_sound->pause();
}

u32 AudioSource::getChannelCount() const
{
	if(m_streamed)
		return m_streamedSound->getChannelCount();
	else
		return m_sound->getBuffer()->getChannelCount();
}

u32 AudioSource::getSampleRate() const
{
	if(m_streamed)
		return m_streamedSound->getSampleRate();
	else
		return m_sound->getBuffer()->getSampleRate();
}

void AudioSource::setPlayingOffset(sf::Time timeOffset)
{
	if(m_streamed)
		m_streamedSound->setPlayingOffset(timeOffset);
	else
		m_sound->setPlayingOffset(timeOffset);
}

sf::Time AudioSource::getPlayingOffset() const
{
	if(m_streamed)
		return m_streamedSound->getPlayingOffset();
	else
		return m_sound->getPlayingOffset();
}

void AudioSource::setLoop(bool loop)
{
	if(m_streamed)
		m_streamedSound->setLoop(loop);
	else
		m_sound->setLoop(loop);
}

bool AudioSource::getLoop() const
{
	if(m_streamed)
		return m_streamedSound->getLoop();
	else
		return m_sound->getLoop();
}

sf::SoundSource::Status AudioSource::getStatus() const
{
	if(m_streamed)
		return m_streamedSound->getStatus();
	else
		return m_sound->getStatus();
}

void AudioSource::setPitch(f32 pitch)                        { internalSource().setPitch(pitch); }
void AudioSource::setVolume(f32 volume)                      { internalSource().setVolume(volume); }
void AudioSource::setPosition(f32 x, f32 y, f32 z)           { internalSource().setPosition(x, y, z); }
void AudioSource::setPosition(const sf::Vector3f &position)  { internalSource().setPosition(position); }
void AudioSource::setRelativeToListener(bool relative)       { internalSource().setRelativeToListener(relative); }
void AudioSource::setMinDistance(f32 distance)               { internalSource().setMinDistance(distance); }
void AudioSource::setAttenuation(f32 attenuation)            { internalSource().setAttenuation(attenuation); }

f32 AudioSource::getPitch() const                            { return internalSource().getPitch(); }
f32 AudioSource::getVolume() const                           { return internalSource().getVolume(); }
sf::Vector3f AudioSource::getPosition() const                { return internalSource().getPosition(); }
bool AudioSource::isRelativeToListener() const               { return internalSource().isRelativeToListener(); }
f32 AudioSource::getMinDistance() const                      { return internalSource().getMinDistance(); }
f32 AudioSource::getAttenuation() const                      { return internalSource().getAttenuation(); }

} // namespace zn



