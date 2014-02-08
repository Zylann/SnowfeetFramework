#ifndef HEADER_ZN_SPRITEANIMATOR_HPP_INCLUDED
#define HEADER_ZN_SPRITEANIMATOR_HPP_INCLUDED

#include <fm/proto/animation/Animator.hpp>
#include <fm/asset/TextureAtlas.hpp>

namespace zn
{

class ZN_API SpriteAnimator : public AAnimator
{
public:

	ZN_COMPONENT(zn::SpriteAnimator, CG_ANIMATOR, 0)

	SpriteAnimator() : AAnimator(),
		r_sequence(0),
		m_sequenceFrame(0),
		m_remainingLoops(0)
	{}

	void setFrame(const std::string & id);

	void play(const std::string & id, s32 loops=-1) override;
	void stop() override;

	inline bool playing() const { return r_sequence != nullptr; }

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

protected:

	void init() override;
	void onUpdate() override;

private:

	const TextureAtlas::Sequence * r_sequence;
	u32 m_sequenceFrame;
	s32 m_remainingLoops;
	sf::Clock m_sequenceClock; // Measures the time between each frame

	std::string m_sequenceName; // Name of the current playing sequence
	std::string m_frameName; // Name of the current still frame (not sequence)

};

} // namespace zn

#endif // HEADER_ZN_SPRITEANIMATOR_HPP_INCLUDED


