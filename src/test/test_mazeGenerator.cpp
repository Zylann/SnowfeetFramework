#include <SFML/Graphics.hpp>
#include "fm/pcg/MazeGenerator.hpp"
#include "test.hpp"

using namespace zn;

void test_mazeGenerator()
{
	MazeGenerator generator(60,40);

	generator.generate(generator.grid.sizeY()/2, generator.grid.sizeX()/2);

	sf::Image mazeVisualization1;
	generator.bakeTexture(mazeVisualization1);
	mazeVisualization1.saveToFile("test_data/maze_vizualization1.png");
}


