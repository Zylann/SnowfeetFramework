#ifndef HEADER_DEMO_GAME_HPP_INCLUDED
#define HEADER_DEMO_GAME_HPP_INCLUDED

#include <fm/scene/core.hpp>
#include <fm/app/WindowApplication.hpp>
#include <fm/scene/graphics.hpp>

#include "common.hpp"

namespace demo
{

class Game : public zn::WindowApplication
{
public:

	Game() : WindowApplication(),
		r_tilemap(nullptr)
	{}

protected:

	void registerNativeUserClasses(zn::ObjectTypeDatabase & odb) override;

	bool onInit() override;
	void onEvent(sf::Event & e) override;
	void onClose() override;

private:

	zn::MapRenderer * r_tilemap;
	sf::RenderTexture m_renderTexture;

};

} // namespace demo

#endif // HEADER_DEMO_GAME_HPP_INCLUDED



