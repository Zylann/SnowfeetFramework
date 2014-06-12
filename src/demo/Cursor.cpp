#include <fm/asset/AssetBank.hpp>
#include <fm/scene/graphics.hpp>
#include <fm/app/Application.hpp>

#include "Cursor.hpp"
#include "common.hpp"

namespace demo
{

ZN_OBJECT_IMPL(demo::Cursor)

void Cursor::onCreate()
{
	zn::SpriteRenderer * sprite = entity().addComponent<zn::SpriteRenderer>();
	sf::Texture * texture = zn::AssetBank::current()->textures.get("cursor");
	sprite->setTexture(texture);
}

//void Cursor::onStart()
//{
//	Application::instance()->setSystemCursorVisible(false);
//}

void Cursor::onUpdate()
{
	sf::Vector2i mousePos = zn::Application::instance()->mousePosition();
	sf::Vector2i screenSize = zn::Application::instance()->screenSize();
	entity().transform.setPosition(sf::Vector2f(
		mousePos.x - screenSize.x/2,
		mousePos.y - screenSize.y/2
	));
}

} // namespace demo

