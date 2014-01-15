#include "test.hpp"
#include "fm/pcg/AutoTiler.hpp"

using namespace zn;

void test_autotiler()
{
	const u32 width = 16;
	const u32 height = 8;

	// TODO profile AutoTiler to check performance as it may be used on playtime

	// Let's say '1' are walls, '0' are floor and '2' another type of floor.
	u8 rawGrid[width*height] = {
	//                 |
	//                 v
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 0, 0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 2, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, // <--
		1, 1, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

		// The pointed location must be 5 after processing
	};

	// Put the raw array in a 2D array class
	Array2D<u8> grid(width, height);
	u32 len = grid.area();
	for(u32 i = 0; i < len; ++i)
	{
		grid[i] = rawGrid[i];
	}

	// The 0 type always makes the 0 tile
	AutoTiler::TypeRules type0;
	type0.defaultTile = 0;

	// The 1 type makes different tiles depending on its neighbors
	AutoTiler::TypeRules type1;
	type1.defaultTile = 1;
	type1.setLookupsAsOnly(1); // See all types but self as 0
	type1.addCase(
		0, 0, 0,
		0,    0,
		0, 0, 1, // 1 means self type (here walls) and 0 any other
		{5}
	);
	type1.addCase(
		1, 1, 0,
		1,    0,
		1, 1, 0,
		{6}
	);

	AutoTiler tiler;
	tiler.typeRules.push_back(type0);
	tiler.typeRules.push_back(type1);

	// The map is surrounded by walls
	tiler.defaultType = 1;

	// There is no rules for the '2' type, then the default tile should be used
	tiler.defaultTile = 0;

	// Calculate tiles
	Array2D<u32> tiles(width, height, 0);
	tiler.process(grid, tiles);

	// Display output
	for(u32 y = 0; y < tiles.sizeY(); ++y)
	{
		for(u32 x = 0; x < tiles.sizeX(); ++x)
		{
			std::cout << tiles.getNoEx(x, y) << " ";
		}
		std::cout << std::endl;
	}
}

