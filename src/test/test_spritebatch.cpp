#include <iostream>
#include "test.hpp"
#include "fm/sfml/SpriteBatch.hpp"

using namespace std;
using namespace zn;

void test_spritebatch()
{
	sf::Texture texture1, texture2;
	if(!texture1.loadFromFile("test_data/cat.png") ||
		!texture2.loadFromFile("test_data/bigcat.jpg"))
	{
		cout << "E: Unable to load textures" << endl;
		return;
	}

	sf::RenderWindow window(sf::VideoMode(800,600), "Test SpriteBatch");

	SpriteBatch batch;
	sf::Clock clock;

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		window.clear(sf::Color(128,128,255));

		batch.setRenderTarget(window);
		for(s32 i = 0; i < 10; ++i)
		{
			batch.drawSprite(&texture2,
								sf::Vector2f(50*i,50*i),
								sf::Vector2f(1.f/(f32)i, 1.f/(f32)i));
		}
		float sine = sin(10.f*clock.getElapsedTime().asSeconds());
		batch.drawSprite(&texture1, sf::FloatRect(0,0,16,16), sf::FloatRect(400,10+10.f*sine,64,64), sf::Color(0,255,0));
		batch.display();

		window.display();
	}
}

