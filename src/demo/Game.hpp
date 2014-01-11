#ifndef HEADER_DEMO_GAME_HPP_INCLUDED
#define HEADER_DEMO_GAME_HPP_INCLUDED

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

#endif // HEADER_DEMO_GAME_HPP_INCLUDED



