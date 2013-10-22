#ifndef HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED
#define HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED

#include <unordered_set>
#include <SFML/Audio.hpp>
#include "Component.hpp"

namespace zn
{

// Adds the ability of emitting sounds to an entity.
class AudioEmitter : public AComponent
{
public:

	ZN_COMPONENT(zn::AudioEmitter, CG_AUDIO_EMITTER)

	AudioEmitter() : AComponent(),
		m_minRadius(1000.f),
		m_maxRadius(2000.f)
	{}

	void onAdd(Entity * e) override;
	void onDestroy() override;
	void update() override;

	// Sets attenuation radii :
	// minRadius defines a circle where sound is not attenuated,
	// maxRadius defines the limit in wich the sound is played.
	void setRadii(f32 minRadius, f32 maxRadius);

	// Sets the attenuation factor when the sound is between the min and max
	// radius. Default is 1.
	void setAttenuation(f32 factor);

	// Plays a sound. It will stack with previously played ones if possible.
	// Volume and pitch are in [0,1].
	// Panning is automatically computed from the position of the entity.
	void play(std::string soundName, f32 volume=1, f32 pitch=1, bool loop=false);

	// Stops all the sounds the emitter is playing.
	void stop();

	// TODO support mix streams and non-stream sounds

private:

	friend class AudioSystem;

	void detachSource(sf::Sound *s);
	sf::Sound * getFreeSource();

	f32                             m_minRadius;
	f32                             m_maxRadius;
	f32                             m_attenuation;
	std::unordered_set<sf::Sound*>  m_sourceRefs;

};

} // namespace zn

#endif // HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED


