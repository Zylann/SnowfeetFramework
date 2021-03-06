#ifndef HEADER_ZN_AUDIOEMITTER_HPP_INCLUDED
#define HEADER_ZN_AUDIOEMITTER_HPP_INCLUDED

#include <unordered_set>
#include <fm/sfml/AudioSource.hpp>
#include <fm/scene/core/Component.hpp>

namespace zn
{

// Adds the ability of emitting sounds to an entity.
class ZN_API AudioEmitter : public Component
{
public:

	ZN_OBJECT(zn::AudioEmitter, zn::Component)

	AudioEmitter() : Component(),
		m_spatialize(true),
		m_minRadius(1000.f),
		m_maxRadius(2000.f)
	{}

	void onAdd(Entity * e) override;
	void onDestroy() override;
	void onUpdate() override;

	// Sets attenuation radii :
	// minRadius defines a circle where sound is not attenuated,
	// maxRadius defines the limit in wich the sound is played.
	void setRadii(f32 minRadius, f32 maxRadius);

	void setPosition(const sf::Vector2f & newPosition);

	// Sets the attenuation factor when the sound is between the min and max
	// radius. Default is 1.
	void setAttenuation(f32 factor);

	// Plays a sound that is fully loaded and decompressed in memory.
	// It will stack with previously played ones if possible.
	// Volume and pitch are in [0,1].
	// Panning is automatically computed from the position of the entity.
	void playBuffer(std::string name, f32 volume=1, f32 pitch=1, bool loop=false);

	// Plays a streamed sound. It will stack with previously played ones if possible.
	// Volume and pitch are in [0,1].
	// Panning is automatically computed from the position of the entity.
	void playStream(std::string name, f32 volume=1, f32 pitch=1, bool loop=false);

	// Stops all the sounds the emitter is playing.
	void stop();

	// Sets wether the emitter should play spatialized sounds or not
	void setSpatialized(bool spatialize);

	inline bool spatialized() const { return m_spatialize; }
	inline f32 minRadius() const { return m_minRadius; }
	inline f32 maxRadius() const { return m_maxRadius; }
	inline f32 attenuation() const { return m_attenuation; }

	bool canBeHeard();

	// TODO support mix streams and non-stream sounds

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

private:

	friend class AudioSystem;

	void detachSource(AudioSource *s);
	AudioSource * getFreeSource();

	bool m_spatialize;
	f32 m_minRadius;
	f32 m_maxRadius;
	f32 m_attenuation;
	sf::Vector3f m_position;

	std::unordered_set<AudioSource*> m_sourceRefs;

};

} // namespace zn

#endif // HEADER_ZN_AUDIOEMITTER_HPP_INCLUDED


