#include <SFML/Graphics.hpp>

#include "fm/util/math.hpp"
#include "fm/util/SampledCurve.hpp"

using namespace zn;

void test_sampledcurve()
{
	SampledCurve<f32, 360> sampledSinus(cos, 0, 2.0*math::PI);
	SampledCurve<f32, 360> sampledCosinus(sin, 0, 2.0*math::PI);

	sf::RenderWindow window(sf::VideoMode(800,600), "Test SampledCurve");

	sf::Clock clock;

	sf::VertexArray vertices;
	vertices.resize(2);
	vertices.setPrimitiveType(sf::PrimitiveType::Lines);

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

		window.clear(sf::Color(255,255,255));

		vertices[0].position = sf::Vector2f(400, 300);
		vertices[0].color = sf::Color::Black;

		f32 angle = 0.2f * clock.getElapsedTime().asSeconds();
		f32 r = 200;
		vertices[1].position = sf::Vector2f(400+r*sampledCosinus.getRepeat(angle), 300+r*sampledSinus.getRepeat(angle));
		vertices[1].color = sf::Color::Black;

		window.draw(vertices);

		window.display();
	}
}


