#include <iostream>

#include <fm/sfml/sfml2_utils.hpp>
#include <fm/util/Direction.hpp>
#include <fm/asset/AssetBank.hpp>

#include <fm/proto/animation.hpp>
#include <fm/proto/physics.hpp>

#include "Avatar.hpp"
#include "Cat.hpp"

using namespace zn;

namespace demo
{

void Avatar::init()
{
	Character::init();

	Entity & e = entity();

	r_sprite->setAtlas(AssetBank::current()->atlases.get("avatar"));
	r_sprite->setFrame("idle.down");
	//r_sprite->setScale(sf::Vector2f(100.f, 100.f));

	r_mover->setMaxSpeed(2.5f);

	auto collider = e.addComponent<BoxCollider>();
	collider->setRect(sf::FloatRect(4, 7, 24, 24));
	//collider->enable(false);

	e.addComponent<SpriteAnimator>();
	e.addComponent<AudioEmitter>();

	m_pressedLastFrame = false;
}

void Avatar::onUpdate()
{
	// Poll input

	sf::Vector2i v;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		v.x = -1;
		setSpriteDirection(Direction::WEST);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		v.x = 1;
		setSpriteDirection(Direction::EAST);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		v.y = -1;
		setSpriteDirection(Direction::NORTH);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		v.y = 1;
		setSpriteDirection(Direction::SOUTH);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if(!m_pressedLastFrame)
		{
			m_pressedLastFrame = true;
			entity().audio()->playBuffer("cat_meow1");
//			entity().audio()->playStream("theme");

			// TEST
//			MusicSystem & music = entity().scene().audioSystem.music;
//			if(music.playing())
//			{
//				music.stop();
//			}
//			else
//			{
//				music.play("theme");
//			}
		}
	}
	else
	{
		m_pressedLastFrame = false;
	}

	// Move

	const f32 acceleration = 0.5f;
	r_mover->accelerate(acceleration, v);

	// Make the camera follow

	Entity & cam = entity().scene().mainCamera()->entity();
	cam.transform.setPosition(entity().transform.position() + sf::Vector2f(16,16));
	cam.transform.setRotation(sin(0.5f*entity().scene().time().asSeconds()));

	// Character-specific stuff (sprite orientation...)

	Character::onUpdate();
}

void Avatar::onCollisionEnter(const CollisionInfo & info)
{

}

} // namespace demo


