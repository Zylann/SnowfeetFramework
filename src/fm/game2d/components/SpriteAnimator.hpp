#ifndef HEADER_ZN_SPRITEANIMATOR_HPP_INCLUDED
#define HEADER_ZN_SPRITEANIMATOR_HPP_INCLUDED

#include "Animator.hpp"
#include "../../asset/TextureAtlas.hpp"

namespace zn
{

class SpriteAnimator : public AAnimator
{
public:

	ZN_COMPONENT(zn::SpriteAnimator, CG_ANIMATOR)

	SpriteAnimator() : AAnimator(),
		m_sequence(0),
		m_sequenceFrame(0),
		m_remainingLoops(0)
	{}

	void setFrame(const std::string & id);

	void play(const std::string & id, s32 loops=-1) override;
	void stop() override;

	inline bool playing() const { return m_sequence != nullptr; }

protected:

	void init() override;
	void update() override;

private:

	const TextureAtlas::Sequence * m_sequence;
	u32 m_sequenceFrame;
	s32 m_remainingLoops;
	sf::Clock m_sequenceClock;

};

} // namespace zn

#endif // HEADER_ZN_SPRITEANIMATOR_HPP_INCLUDED


