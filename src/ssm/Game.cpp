#include "fm/proto/graphics/SpriteRenderer.hpp"
//#include "fm/proto/components/SpriteAnimator.hpp"
#include "fm/proto/graphics/MapRenderer.hpp"
#include "fm/proto/physics/MapCollider.hpp"
#include "fm/proto/graphics/Camera.hpp"
//#include "fm/proto/components/ParticleSystem.hpp"
#include "Avatar.hpp"
#include "Cat.hpp"
#include "Game.hpp"

namespace ssm
{

void Game::onInit()
{
	// Assets

	m_assets.loadFromJSON("assets/assets.json");

	// Map

	u32 ts = 32;

	Entity * map = m_scene.createEntity("map", sf::Vector2f(0,0));
	const TiledMap * mapAsset = m_assets.maps.get("test");
	MapRenderer * mr = map->addComponent<MapRenderer>();
	mr->setMap(mapAsset, m_assets.atlases.get("room"), "background");
	mr->drawOrder = -1; // The map is drawn first
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

	// Camera

	Entity * cameraObj = m_scene.createEntity("main_camera", sf::Vector2f(500,100));
	Camera * camera = cameraObj->addComponent<Camera>();
	camera->setInternalView(m_window.getDefaultView());
	camera->setScaleMode(Camera::ADAPTED);
	m_scene.setMainCamera(camera);
	m_scene.audioSystem.setListener(cameraObj);

	// Avatar

	Entity * avatar = m_scene.createEntity("the_bum", sf::Vector2f(64*ts,31*ts));
	avatar->addComponent<Avatar>();

	// Cats

	Entity * cat = m_scene.createEntity("snowfeet", sf::Vector2f(62*ts, 31*ts));
	cat->addComponent<Cat>();

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

//	case sf::Event::MouseButtonPressed:
//	{
//		sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
//		sf::Vector2f pos = _window.mapPixelToCoords(mousePos, _scene.mainCamera()->internalView());
//		if(_scene.colliderAt(pos))
//			std::cout << "Collision" << std::endl;
//	}
//	break;

	default:
		break;
	}
}

void Game::onClose()
{

}


} // namespace ssm


