#include <fm/proto/graphics/SpriteRenderer.hpp>
#include <fm/proto/core/Entity.hpp>
#include <fm/asset/AssetBank.hpp>
#include <fm/proto/core/Application.hpp>

#include "Cursor.hpp"
#include "common.hpp"

namespace demo
{

void Cursor::init()
{
	zn::SpriteRenderer * sprite = entity().addComponent<zn::SpriteRenderer>();
	sf::Texture * texture = zn::AssetBank::current()->textures.get("cursor");
	sprite->setTexture(*texture);
}

void Cursor::onStart()
{
	Application::instance()->setSystemCursorVisible(false);
}

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

