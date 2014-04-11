#ifndef HEADER_ZN_AANIMATOR_HPP_INCLUDED
#define HEADER_ZN_AANIMATOR_HPP_INCLUDED

#include <fm/proto/core/Component.hpp>
#include <fm/asset/TextureAtlas.hpp>

namespace zn
{

class ZN_API AAnimator : public AComponent
{
public:

	void onAdd(Entity * e) override;
	void onDestroy() override;

	// Starts an animation defined in the texture atlas.
	// \param loops: how many times the animation must loop. -1 means infinite, 0 means one-shot.
	virtual void play(const std::string & id, s32 loops=-1) = 0;

	// Stops all animations.
	// Does nothing if there is no animation playing.
	virtual void stop() = 0;

	virtual void onAtlasChanged(const TextureAtlas * newAtlas) {}

};

} // namespace zn

#endif // HEADER_ZN_AANIMATOR_HPP_INCLUDED

