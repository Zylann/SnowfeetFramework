#ifndef HEADER_DEMO_CHARACTER_HPP_INCLUDED
#define HEADER_DEMO_CHARACTER_HPP_INCLUDED

#include <fm/scene/graphics.hpp>
#include <fm/util/Direction.hpp>

#include "common.hpp"
#include "Mover.hpp"

namespace demo
{

// Base component for characters that can move and animate
class Character : public zn::Behaviour
{
public:

	ZN_COMPONENT(demo::Character, zn::Behaviour, 0)

	Character() : Behaviour(),
		r_sprite(nullptr),
		r_mover(nullptr),
		m_spriteDir(Direction::DOWN)
	{}

	virtual void onCreate() override;

	inline u8 spriteDirection() const { return m_spriteDir; }
	void setSpriteDirection(u8 dir);

protected:

	SpriteRenderer * r_sprite;
	Mover *          r_mover;

	void onUpdate() override;

private:

	u8    m_spriteDir;
	u8    m_spriteDirLastFrame;
	bool  m_movingLastFrame;

};

} // namespace demo

#endif // HEADER_DEMO_CHARACTER_HPP_INCLUDED


