#include <fm/proto/graphics.hpp>
#include <fm/proto/physics.hpp>

#include "Avatar.hpp"
#include "Cat.hpp"
#include "Game.hpp"
#include "Cursor.hpp"
#include "Rotate.hpp"

namespace demo
{

void Game::registerNativeUserComponents(zn::ComponentFactory & f)
{
	f.registerType<Avatar>();
	f.registerType<Cat>();
	f.registerType<Cursor>();
	f.registerType<Mover>();
	f.registerType<Rotate>();
}

bool Game::onInit()
{
	// Scene config

	// Setup a GUI layer over the default layer
	m_scene.layers.setLayer(1, "gui", 1);

	// Map

	f32 ts = 1;//32; // tile size

	Entity * map = m_scene.createEntity("map", sf::Vector2f(0,0));
	const TiledMap * mapAsset = m_assets.maps.get("test_map");
	r_tilemap = map->addComponent<MapRenderer>();
	r_tilemap->build(mapAsset, m_assets.textures.get("room"), m_assets.atlases.get("room"), "background");
	r_tilemap->drawOrder = -10; // The map is drawn first
	MapCollider * mc = map->addComponent<MapCollider>();
	mc->setColliderType(1, sf::FloatRect(0, 0, ts, ts));
	mc->setColliderType(2, sf::FloatRect(0, 0, ts/2.f, ts));
	mc->setColliderType(3, sf::FloatRect(ts/2.f, 0, ts/2.f, ts));
	mc->setColliderType(4, sf::FloatRect(0, 0, ts, ts/2.f));
	mc->setColliderType(5, sf::FloatRect(0, ts/2.f, ts, ts/2.f));
	mc->setColliderType(6, sf::FloatRect(0, 0, ts/2.f, ts/2.f));
	mc->setColliderType(7, sf::FloatRect(ts/2.f, 0, ts/2.f, ts/2.f));
	mc->setColliderType(8, sf::FloatRect(0, ts/2.f, ts/2.f, ts/2.f));
	mc->setColliderType(9, sf::FloatRect(ts/2.f, ts/2.f, ts/2.f, ts/2.f));
	mc->setColliderType(10, sf::FloatRect(ts/4.f, ts/4.f, ts/2.f, ts/2.f));
	mc->setColliderType(11, sf::FloatRect(0, ts/4.f, ts, ts/2.f));
	mc->setColliderType(12, sf::FloatRect(ts/4.f, 0, ts/2.f, ts));
	mc->build(*mapAsset, "collision", "collision");

	// Scene camera

	const sf::View & defaultView = m_window.getDefaultView();

	Entity * cameraObj = m_scene.createEntity("main_camera");
	Camera * camera = cameraObj->addComponent<Camera>();
	camera->setInternalView(defaultView);
	camera->setSize(sf::Vector2f(20, 12));
	camera->setScaleMode(Camera::ADAPTED);
	camera->clearColor = sf::Color(8, 16, 32);
	m_scene.setMainCamera(camera);
	m_scene.audioSystem.setListener(cameraObj);

	// GUI camera

	Entity * guiCameraObj = m_scene.createEntity("gui_camera");
	Camera * guiCamera = guiCameraObj->addComponent<Camera>();
	guiCamera->setInternalView(defaultView);
	guiCamera->setScaleMode(Camera::FIXED);
	guiCamera->doClear = false;
	guiCamera->depth = 2;
	//guiCamera->topLeftOrigin = true;
	guiCamera->layerMask = 1 << m_scene.layers["gui"]->index;

	// Secondary camera

	if(!m_renderTexture.create(256, 256))
	{
		std::cout << "Failed to create render texture" << std::endl;
		return false;
	}

	Entity * secondaryCamObj = m_scene.createEntity("secondary_camera", sf::Vector2f(64,20));
	Camera * secondaryCam = secondaryCamObj->addComponent<Camera>();
	secondaryCam->setScaleMode(Camera::FIXED);
	secondaryCam->setFixedZoom(32);
	secondaryCam->setRenderTexture(&m_renderTexture);
	secondaryCam->depth = 1;

	// Sprite displaying the secondary camera

	Entity * monitor = m_scene.createEntity("monitor", sf::Vector2f(55, 17));
	SpriteRenderer * sprite = monitor->addComponent<SpriteRenderer>();
	sprite->scaleToPixels(false);
	sprite->setScale(sf::Vector2f(16,10));
	sprite->setTexture(&m_renderTexture.getTexture());
	sprite->drawOrder = -1;
	sprite->setColor(sf::Color(200,200,200));

	// Mouse cursor

	Entity * mouseCursor = m_scene.createEntity("cursor");
	mouseCursor->addComponent<Cursor>();
	mouseCursor->setLayerByName("gui");

	// Avatar

	Entity * avatar = m_scene.createEntity("avatar", sf::Vector2f(64,31));
	avatar->addComponent<Avatar>();

	// Cats

	Entity * cat = m_scene.createEntity("cat", sf::Vector2f(62, 31));
	cat->addComponent<Cat>();

	// Text

	Entity * textEntity = m_scene.createEntity("text", sf::Vector2f(-200, -200));
	textEntity->setLayerByName("gui");
	TextRenderer * text = textEntity->addComponent<TextRenderer>();
	text->setText(L"Hello World ! éèë");
	text->setFont("default");
	text->setCharacterSize(16);
	text->setColor(sf::Color(255,192,32));

	// Rotating things

	const sf::Texture * ballTexture = m_assets.textures.get("wool_ball");

	Entity * rotateObj1 = m_scene.createEntity("ball1", sf::Vector2f(50*ts, 31*ts));
	rotateObj1->addComponent<Rotate>();
	sprite = rotateObj1->addComponent<SpriteRenderer>();
	sprite->scaleToPixels(false);
	sprite->setTexture(ballTexture);

	Entity * rotateObj2 = m_scene.createEntity("ball2", sf::Vector2f(2*ts, 0));
	rotateObj2->addComponent<Rotate>();
	sprite = rotateObj2->addComponent<SpriteRenderer>();
	sprite->scaleToPixels(false);
	sprite->setTexture(ballTexture);
	rotateObj2->transform.setParent(rotateObj1->transform);

	Entity * rotateObj3 = m_scene.createEntity("ball3", sf::Vector2f(ts, 0));
	sprite = rotateObj3->addComponent<SpriteRenderer>();
	sprite->scaleToPixels(false);
	sprite->setTexture(ballTexture);
	rotateObj3->transform.setParent(rotateObj2->transform);

	// Shader test

	Entity * snowTV = m_scene.createEntity("shader_test", sf::Vector2f(65*ts, 31*ts));
	snowTV->transform.setScale(sf::Vector2f(3,3));
	sprite = snowTV->addComponent<SpriteRenderer>();
	sprite->scaleToPixels(false);
	sprite->setTexture(ballTexture);
	sprite->setMaterial(m_assets.materials.get("noise"));

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
		else if(e.key.code == sf::Keyboard::Key::T)
		{
			// Test entity destruction with parenting
			Entity * e = m_scene.findEntityFromName("ball3");
			if(e)
			{
				e->destroyLater();
			}
		}
		else if(e.key.code == sf::Keyboard::Key::F4)
		{
			setFullScreen(!fullScreen());
		}
		else if(e.key.code == sf::Keyboard::Key::F5)
		{
			// TEST
			m_scene.saveToFile("scene_dump.json");
		}
		break;

	case sf::Event::MouseButtonPressed:
	{
		sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
		sf::Vector2f pos = m_window.mapPixelToCoords(mousePos, m_scene.mainCamera()->internalView());
		s32 tileX = pos.x / (f32)r_tilemap->sceneTileSize().x;
		s32 tileY = pos.y / (f32)r_tilemap->sceneTileSize().y;
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


