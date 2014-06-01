#include <iostream>

#include <fm/scene/animation/SpriteAnimator.hpp>
#include <fm/asset/TextureAtlas.hpp>
#include <fm/scene/graphics/Renderer.hpp>
#include <fm/scene/core/Entity.hpp>

using namespace std;

namespace zn
{

//------------------------------------------------------------------------------
void SpriteAnimator::setFrame(const std::string& id)
{
	// Get renderer
	Renderer * r = entity().renderer();
	// If there is a renderer with an atlas
	if(r != nullptr && r->atlas() != nullptr)
	{
		// Get the frame in the atlas
		const TextureAtlas::Frame * f = r->atlas()->frame(id);
		if(f != nullptr)
		{
			// Set new UVs on the sprite
			r->setTextureRect(f->rect);
			m_frameName = id;

			// Stop the animation if it was playing
			stop();
		}
	}
}

//------------------------------------------------------------------------------
void SpriteAnimator::onAtlasChanged(const TextureAtlas * newAtlas)
{
	if(newAtlas == nullptr)
		stop();
	if(r_sequence != nullptr)
	{
		r_sequence = newAtlas->sequence(m_sequenceName);
		if(r_sequence == nullptr)
		{
			stop();
			m_sequenceName = "";
		}
	}
}

//------------------------------------------------------------------------------
void SpriteAnimator::play(const std::string& id, s32 loops)
{
	// Ensure there is a renderer
	Renderer * r = entity().renderer();
	if(r == nullptr)
		return;

	// Get the atlas
	const TextureAtlas * atlas = r->atlas();
	if(atlas != nullptr)
	{
		// Get sequence
		const TextureAtlas::Sequence * s = atlas->sequence(id);
		if(s != nullptr)
		{
			// Reference sequence and initialize animation
			r_sequence = s;
			r->setTextureRect(r_sequence->frames[0].rect);
			m_sequenceClock.restart();
			m_sequenceFrame = 0;
			m_remainingLoops = loops;

			m_sequenceName = id;
			//m_frameName = "";
		}
#ifdef ZN_DEBUG
		else
		{
			cout << "E: SpriteAnimator::play: "
				"not found (" << id << ") on entity \"" << entity().name() << '"' << endl;

		}
#endif
	}
#ifdef ZN_DEBUG
	else
	{
		cout << "E: SpriteAnimator::play: "
			"no TextureAtlas ! (entity \"" << entity().name() << "\")" << endl;
	}
#endif
}

//------------------------------------------------------------------------------
void SpriteAnimator::stop()
{
	r_sequence = nullptr;
}

//------------------------------------------------------------------------------
void SpriteAnimator::onCreate()
{
}

//------------------------------------------------------------------------------
void SpriteAnimator::onUpdate()
{
	Renderer * r = entity().renderer();
	if(r == nullptr)
		return;

	if(r_sequence != nullptr)
	{
		u32 time = m_sequenceClock.getElapsedTime().asMilliseconds();
		if(time >= r_sequence->frames[m_sequenceFrame].duration)
		{
			// Jump to next frame

			++m_sequenceFrame;
			if(m_sequenceFrame == r_sequence->frames.size())
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

			if(r_sequence != nullptr)
			{
				r->setTextureRect(r_sequence->frames[m_sequenceFrame].rect);
				m_sequenceClock.restart();
			}
		}
	}
}

//------------------------------------------------------------------------------
void SpriteAnimator::serializeData(JsonBox::Value & o)
{
	Animator::serializeData(o);

	o["sequence"]        = r_sequence!=nullptr ? m_sequenceName : "";
	o["sequenceFrame"]   = r_sequence!=nullptr ? (s32)m_sequenceFrame : (s32)0;
	o["remainingLoops"]  = r_sequence!=nullptr ? m_remainingLoops : 0;
	o["frame"]           = r_sequence==nullptr ? m_frameName : "";
}

//------------------------------------------------------------------------------
void SpriteAnimator::unserializeData(JsonBox::Value & o)
{
	Animator::unserializeData(o);

	m_sequenceName    = o["sequence"].getString();
	m_sequenceFrame   = o["sequenceFrame"].getInt();
	m_remainingLoops  = o["remainingLoops"].getInt();
	m_frameName       = o["frame"].getString();

	r_sequence = nullptr;
}

//------------------------------------------------------------------------------
void SpriteAnimator::postUnserialize()
{
	// Relink sequence
	if(!m_sequenceName.empty() && r_sequence == nullptr)
	{
		Renderer * r = entity().renderer();
		r_sequence = r->atlas()->sequence(m_sequenceName);
	}
}

} // namespace zn



