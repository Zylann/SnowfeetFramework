#ifndef SSM_HEADER_GAME_HPP_INCLUDED
#define SSM_HEADER_GAME_HPP_INCLUDED

#include "fm/proto/GameApp.hpp"
#include "common.hpp"
#include "fm/proto/graphics/MapRenderer.hpp"

namespace demo
{

class Game : public zn::GameApp
{
public:

	Game() : GameApp(),
		r_tilemap(nullptr)
	{}

protected:

	virtual bool onInit();
	virtual void onEvent(sf::Event & e);
	virtual void onClose();

private:

	zn::MapRenderer * r_tilemap;

};

} // namespace demo

#endif // SSM_HEADER_GAME_HPP_INCLUDED



