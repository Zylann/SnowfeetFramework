#ifndef HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED
#define HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED

#include <unordered_set>
#include <SFML/Audio.hpp>
#include "Component.hpp"

namespace zn
{

class AudioEmitter : public AComponent
{
public:

	ZN_COMPONENT(zn::AudioEmitter, CG_AUDIO_EMITTER)

	AudioEmitter() : AComponent(),
		m_minRadius(4),
		m_maxRadius(16)
	{}

	void onAdd(Entity * e) override;
	void onDestroy() override;
	void update() override;

	void setRadii(f32 minRadius, f32 maxRadius);
	void play(std::string soundName, f32 volume=1, f32 pitch=1, bool loop=false);
	void stop();

private:

	friend class AudioSystem;

	void detachSource(sf::Sound *s);
	sf::Sound * getFreeSource();

	f32                             m_minRadius;
	f32                             m_maxRadius;
	std::unordered_set<sf::Sound*>  m_sourceRefs;

};

} // namespace zn

#endif // HEADER_ZN_AUDIOSOURCE_HPP_INCLUDED


