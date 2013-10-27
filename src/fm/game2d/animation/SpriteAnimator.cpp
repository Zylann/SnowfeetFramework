#include <iostream>

#include "SpriteAnimator.hpp"
#include "../../asset/TextureAtlas.hpp"
#include "../graphics/Renderer.hpp"
#include "../Entity.hpp"

using namespace std;

namespace zn
{

void SpriteAnimator::setFrame(const std::string& id)
{
	ARenderer * r = entity().renderer();
	if(r != nullptr && r->atlas() != nullptr)
	{
		const TextureAtlas::Frame * f = r->atlas()->frame(id);
		if(f != nullptr)
			r->setTextureRect(f->rect);
	}
}

void SpriteAnimator::play(const std::string& id, s32 loops)
{
	ARenderer * r = entity().renderer();
	if(r == nullptr)
		return;

	const TextureAtlas * atlas = r->atlas();

	if(atlas != nullptr)
	{
		const TextureAtlas::Sequence * s = atlas->sequence(id);
		if(s != nullptr)
		{
			m_sequence = s;
			r->setTextureRect(m_sequence->frames[0].rect);
			m_sequenceClock.restart();
			m_sequenceFrame = 0;
			m_remainingLoops = loops;
		}
#ifdef ZN_DEBUG
		else
			cout << "E: SpriteAnimator::play: not found (" << id << ") on entity \"" << entity().name() << '"' << endl;
#endif
	}
#ifdef ZN_DEBUG
	else
		cout << "E: SpriteAnimator::play: no TextureAtlas !" << endl;
#endif
}

void SpriteAnimator::stop()
{
	m_sequence = nullptr;
}

void SpriteAnimator::init()
{
}

void SpriteAnimator::update()
{
	ARenderer * r = entity().renderer();
	if(r == nullptr)
		return;

	if(m_sequence != nullptr)
	{
		u32 time = m_sequenceClock.getElapsedTime().asMilliseconds();
		if(time >= m_sequence->frames[m_sequenceFrame].duration)
		{
			++m_sequenceFrame;
			if(m_sequenceFrame == m_sequence->frames.size())
			{
				m_sequenceFrame = 0;
				if(m_remainingLoops == 0)
				{
					stop();
				}
				else if(m_remainingLoops > 0)
				{
					--m_remainingLoops;
				}
			}

			// FIXME potential sigsev due to nulling _sequenceFrame before !
			r->setTextureRect(m_sequence->frames[m_sequenceFrame].rect);
			m_sequenceClock.restart();
		}
	}
}

} // namespace zn



