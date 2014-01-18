#ifndef HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED
#define HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <fm/types.hpp>

namespace zn
{

// Merges streamed and full-loaded audio file playback,
// because SFML has them separated, despite their interface's high similarity.
// Yes, It's... basically a wrapper.
class AudioSource
{
public:

	AudioSource() :
		m_streamedSound(nullptr),
		m_streamed(false)
	{
		m_sound = new sf::Sound();
	}

	~AudioSource()
	{
		if(m_sound != nullptr)
		{
			delete m_sound;
		}
		if(m_streamedSound != nullptr)
		{
			delete m_streamedSound;
		}
	}

	inline bool streamed() const { return m_streamed; }

	void setBuffer(const sf::SoundBuffer & buffer);
	void setStreamFromFile(const std::string & filePath);

	// TODO implement this to allow custom streams (synthetisers?)
	//void setStream(sf::SoundStream * stream);

	void play();
	void pause();
	void stop();
	void setPlayingOffset(sf::Time timeOffset);
	sf::Time getPlayingOffset() const;

	sf::Time getDuration() const;
	u32 getChannelCount() const;
	u32 getSampleRate() const;

	void setLoop(bool loop);
	bool getLoop() const;

	//----------------------------------
	// SoundSource wrap methods
	//----------------------------------

	void setPitch(f32 pitch);
	void setVolume(f32 volume);
	void setPosition(f32 x, f32 y, f32 z);
	void setPosition(const sf::Vector3f &position);
	void setRelativeToListener (bool relative);
	void setMinDistance (f32 distance);
	void setAttenuation (f32 attenuation);

	f32 getPitch() const;
	f32 getVolume() const;
	sf::Vector3f getPosition() const;
	bool isRelativeToListener() const;
	f32 getMinDistance() const;
	f32 getAttenuation() const;

	sf::SoundSource::Status getStatus() const;

private:

	inline sf::SoundSource & internalSource() const
	{
		if(m_streamed)
			return *m_streamedSound;
		else
			return *m_sound;
	}

	sf::Sound *   m_sound;
	sf::Music *   m_streamedSound;
	bool          m_streamed;

};

} // namespace zn

#endif // HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED


