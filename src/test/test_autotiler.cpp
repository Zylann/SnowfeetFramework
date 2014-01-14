#include "test.hpp"
#include "fm/pcg/AutoTiler.hpp"

using namespace zn;

void test_autotiler()
{
	const u32 width = 16;
	const u32 height = 8;

	// Let's say '1' are walls and '0' are floor.
	u8 rawGrid[width*height] = {
	//                 |
	//                 v
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, // <--
		1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

		// The pointed coordinates must be 5 after processing
	};

	Array2D<u8> grid(width, height);
	u32 len = grid.area();
	for(u32 i = 0; i < len; ++i)
	{
		grid[i] = rawGrid[i];
	}

	Array2D<u32> tiles(width, height, 0);

	// The 0 type always makes the 0 tile
	AutoTiler::TypeRules type0;
	type0.defaultTile = 0;

	// The 1 type makes fifferent tiles depending on its neighbors
	AutoTiler::TypeRules type1;
	type1.defaultTile = 1;
	type1.add(
		0, 0, 0,
		0,    0,
		0, 0, 1,
		{5}
	);
	type1.add(
		1, 1, 0,
		1,    0,
		1, 1, 0,
		{6}
	);

	AutoTiler tiler;
	tiler.typeRules.push_back(type0);
	tiler.typeRules.push_back(type1);
	tiler.defaultType = 1;

	tiler.process(grid, tiles);

	for(u32 y = 0; y < tiles.sizeY(); ++y)
	{
		for(u32 x = 0; x < tiles.sizeX(); ++x)
		{
			std::cout << tiles.getNoEx(x, y) << " ";
		}
		std::cout << std::endl;
	}
}

