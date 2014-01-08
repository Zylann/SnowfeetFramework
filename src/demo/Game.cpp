#include "fm/proto/graphics/SpriteRenderer.hpp"
//#include "fm/proto/components/SpriteAnimator.hpp"
#include "fm/proto/graphics/MapRenderer.hpp"
#include "fm/proto/physics/MapCollider.hpp"
#include "fm/proto/graphics/Camera.hpp"
#include "fm/proto/graphics/TextRenderer.hpp"
//#include "fm/proto/components/ParticleSystem.hpp"
#include "Avatar.hpp"
#include "Cat.hpp"
#include "Game.hpp"
#include "Cursor.hpp"

namespace demo
{

bool Game::onInit()
{
	// Scene config

	m_scene.layers.setLayer(1, "gui");

	// Map

	u32 ts = 32;

	Entity * map = m_scene.createEntity("map", sf::Vector2f(0,0));
	const TiledMap * mapAsset = m_assets.maps.get("test");
	r_tilemap = map->addComponent<MapRenderer>();
	r_tilemap->build(mapAsset, m_assets.atlases.get("room"), "background");
	r_tilemap->drawOrder = -1; // The map is drawn first
	MapCollider * mc = map->addComponent<MapCollider>();
	mc->setColliderType(1, sf::FloatRect(0, 0, ts, ts));
	mc->setColliderType(2, sf::FloatRect(0, 0, ts/2, ts));
	mc->setColliderType(3, sf::FloatRect(ts/2, 0, ts/2, ts));
	mc->setColliderType(4, sf::FloatRect(0, 0, ts, ts/2));
	mc->setColliderType(5, sf::FloatRect(0, ts/2, ts, ts/2));
	mc->setColliderType(6, sf::FloatRect(0, 0, ts/2, ts/2));
	mc->setColliderType(7, sf::FloatRect(ts/2, 0, ts/2, ts/2));
	mc->setColliderType(8, sf::FloatRect(0, ts/2, ts/2, ts/2));
	mc->setColliderType(9, sf::FloatRect(ts/2, ts/2, ts/2, ts/2));
	mc->setColliderType(10, sf::FloatRect(ts/4, ts/4, ts/2, ts/2));
	mc->setColliderType(11, sf::FloatRect(0, ts/4, ts, ts/2));
	mc->setColliderType(12, sf::FloatRect(ts/4, 0, ts/2, ts));
	mc->build(*mapAsset, "collision", "collision");

	// Scene camera

	const sf::View & defaultView = m_window.getDefaultView();

	Entity * cameraObj = m_scene.createEntity("main_camera", sf::Vector2f(500,100));
	Camera * camera = cameraObj->addComponent<Camera>();
	camera->setInternalView(defaultView);
	camera->setScaleMode(Camera::ADAPTED);
	m_scene.setMainCamera(camera);
	m_scene.audioSystem.setListener(cameraObj);

	// GUI camera

	Entity * guiCameraObj = m_scene.createEntity("gui_camera");
	Camera * guiCamera = guiCameraObj->addComponent<Camera>();
	guiCamera->setInternalView(defaultView);
	guiCamera->setScaleMode(Camera::FIXED);
	guiCamera->depth = 1;
	//guiCamera->topLeftOrigin = true;
	guiCameraObj->setLayerMask(m_scene.layers.maskFromName("gui"));

	// Mouse cursor

	Entity * mouseCursor = m_scene.createEntity("cursor");
	mouseCursor->addComponent<Cursor>();
	mouseCursor->setLayerMask(m_scene.layers.maskFromName("gui"));

	// Avatar

	Entity * avatar = m_scene.createEntity("the_bum", sf::Vector2f(64*ts,31*ts));
	avatar->addComponent<Avatar>();

	// Cats

	Entity * cat = m_scene.createEntity("snowfeet", sf::Vector2f(62*ts, 31*ts));
	cat->addComponent<Cat>();

	// Text

	Entity * textEntity = m_scene.createEntity("text", sf::Vector2f(-200, -200));
	textEntity->setLayerMask(m_scene.layers.maskFromName("gui"));
	TextRenderer * text = textEntity->addComponent<TextRenderer>();
	text->setText(L"Hello World ! éèë");
	text->setFont("default");
	text->setCharacterSize(16);
	text->setColor(sf::Color(255,192,32));

	return true;
}

void Game::onEvent(sf::Event& e)
{
	switch(e.type)
	{
	case sf::Event::KeyPressed:
		if(e.key.code == sf::Keyboard::Key::Escape)
		{
			stop();
		}
		else if(e.key.code == sf::Keyboard::Key::F4)
		{
			setFullScreen(!fullScreen());
		}
		else if(e.key.code == sf::Keyboard::Key::F5)
		{
			// TEST
			m_scene.saveToFile("serialized_scene_test.json");
		}
		break;

	case sf::Event::MouseButtonPressed:
	{
		sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
		sf::Vector2f pos = m_window.mapPixelToCoords(mousePos, m_scene.mainCamera()->internalView());
		s32 tileX = pos.x / (f32)r_tilemap->tileSize().x;
		s32 tileY = pos.y / (f32)r_tilemap->tileSize().y;
		if(r_tilemap->tiles.contains(tileX, tileY))
		{
			r_tilemap->setTile(tileX, tileY, 4);
		}
	}
	break;

	default:
		break;
	}
}

void Game::onClose()
{

}


} // namespace demo


