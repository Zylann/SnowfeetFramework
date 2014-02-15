#ifndef HEADER_DEMO_GAME_HPP_INCLUDED
#define HEADER_DEMO_GAME_HPP_INCLUDED

#include "fm/proto/Application.hpp"
#include "common.hpp"
#include "fm/proto/graphics/MapRenderer.hpp"

namespace demo
{

class Game : public zn::Application
{
public:

	Game() : Application(),
		r_tilemap(nullptr)
	{}

protected:

	void registerNativeUserComponents(zn::ComponentFactory & f) override;

	bool onInit() override;
	void onEvent(sf::Event & e) override;
	void onClose() override;

private:

	zn::MapRenderer * r_tilemap;
	sf::RenderTexture m_renderTexture;

};

} // namespace demo

#endif // HEADER_DEMO_GAME_HPP_INCLUDED



