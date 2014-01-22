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
		1, 1, 1, 0, 0, 3, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1,
		1, 1, 3, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, // <--
		1, 1, 3, 0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 0, 0, 1,
		1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 2, 0, 0, 1,
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
	AutoTiler::RuleSet type0;
	type0.defaultOutput = {0};

	// The 1 type makes different tiles depending on its neighbors
	AutoTiler::RuleSet type1;
	type1.connections.insert(1);
	type1.defaultOutput = {1};
	// 0 0 0
	// 0   0
	// 0 0 1
	type1.addCase(0b00000001, {5});
	// 1 1 0
	// 1   0
	// 1 1 0
	type1.addCase(0b11010110, {6});

	// The 2 type represents the same wall corner in several cases: use of "don't care" neighbors
	AutoTiler::RuleSet type2;
	type2.connections.insert(1); // it connects with 1
	type2.connections.insert(2); // and itself too
	type2.defaultOutput = {2};
	// * 0 *
	// 1   0
	// 1 1 *
	type2.addCase(0b00010110, 0b01011110, {7});

	AutoTiler tiler;
	tiler.addRuleSet(0, type0);
	tiler.addRuleSet(1, type1);
	tiler.addRuleSet(2, type2);

	// The map is surrounded by walls
	tiler.defaultInput = 1;

	// There is no rules for the '3' type, then the default tile should be used
	tiler.defaultOutput = 0;

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

