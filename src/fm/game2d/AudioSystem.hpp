#ifndef HEADER_ZN_AUDIOSYSTEM_HPP_INCLUDED
#define HEADER_ZN_AUDIOSYSTEM_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include "components/AudioEmitter.hpp"

namespace zn
{

class AudioSystem
{
public:

	AudioSystem();
	~AudioSystem();

	static const u32 DEFAULT_HARD_SOURCES_COUNT = 32;

	void registerSource(AudioEmitter * source);
	void unregisterSource(AudioEmitter * source);

	void setListenerDepth(f32 z);
	void setListenerPosition(sf::Vector2f position);
	void setListener(const Entity * e);

	inline const Entity * listener() const { return r_listenerEntity; }

	inline sf::Vector2f listenerPosition() const
	{
		return sf::Vector2f(
			sf::Listener::getPosition().x,
			sf::Listener::getPosition().y
		);
	}

	void update();

//	void playStream(const std::string musicName);
//	void stopStream();
//	void setVolume(float volume);

private:

	friend class AudioEmitter;

	// A sound card can only play a limited amount of sounds at the same time,
	// called "sources" in OpenAL. An AttachedSource represents a sound
	// being played by an emitter component, except that the component can own
	// several hardSources depending on its priority in the soundscape.
	struct AttachedSource
	{
		AudioEmitter * emitterRef;
		sf::Sound source;
	};

	sf::Sound * requestSource(AudioEmitter & newOwner);
	void detachEmitterSources(AudioEmitter & emitter);

	std::unordered_set<AudioEmitter*>   m_emitters;
	std::vector<AttachedSource>         m_sources;
	const Entity *                      r_listenerEntity;

};

} // namespace zn

#endif // HEADER_ZN_AUDIOSYSTEM_HPP_INCLUDED


