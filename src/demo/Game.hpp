#ifndef HEADER_DEMO_GAME_HPP_INCLUDED
#define HEADER_DEMO_GAME_HPP_INCLUDED

#include <fm/scene/core.hpp>
#include <fm/app/Application.hpp>
#include <fm/scene/graphics.hpp>

#include "common.hpp"

namespace demo
{

class Game : public zn::Application
{
public:

	Game() : Application(),
		r_tilemap(nullptr)
	{}

protected:

	void registerNativeUserComponents(zn::ObjectTypeDatabase & odb) override;

	bool onInit() override;
	void onEvent(sf::Event & e) override;
	void onClose() override;

private:

	zn::MapRenderer * r_tilemap;
	sf::RenderTexture m_renderTexture;

};

} // namespace demo

#endif // HEADER_DEMO_GAME_HPP_INCLUDED



