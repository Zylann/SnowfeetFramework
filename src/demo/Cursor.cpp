#include "fm/proto/graphics/SpriteRenderer.hpp"
#include "fm/proto/Entity.hpp"
#include "fm/asset/AssetBank.hpp"
#include "fm/proto/GameApp.hpp"
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
	GameApp::instance()->setSystemCursorVisible(false);
}

void Cursor::onUpdate()
{
	sf::Vector2i mousePos = zn::GameApp::instance()->mousePosition();
	sf::Vector2i screenSize = zn::GameApp::instance()->screenSize();
	entity().transform.setPosition(sf::Vector2f(
		mousePos.x - screenSize.x/2,
		mousePos.y - screenSize.y/2
	));
}

} // namespace demo

