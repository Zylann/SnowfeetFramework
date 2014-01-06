#ifndef SSM_HEADER_GAME_HPP_INCLUDED
#define SSM_HEADER_GAME_HPP_INCLUDED

#include "fm/proto/GameApp.hpp"
#include "common.hpp"

namespace demo
{

class Game : public zn::GameApp
{
public:

	Game() : GameApp()
	{}

protected:

	virtual bool onInit();
	virtual void onEvent(sf::Event & e);
	virtual void onClose();

};

} // namespace demo

#endif // SSM_HEADER_GAME_HPP_INCLUDED



