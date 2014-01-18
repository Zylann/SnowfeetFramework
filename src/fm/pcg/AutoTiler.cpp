#include <fm/pcg/AutoTiler.hpp>

namespace zn
{

// 8-connexity neighboring vectors from center
const s32 g_nv8[8][2] = {
	{-1, -1},
	{0, -1},
	{1, -1},
	{-1, 0},
	{1, 0},
	{-1, 1},
	{0, 1},
	{1, 1}
};

void AutoTiler::process(const Array2D<Type> & typeGrid, Array2D<Tile> & tileGrid)
{
	// For each cell in the grid
	for(s32 y = 0; y < static_cast<s32>(typeGrid.sizeY()); ++y)
	{
		for(s32 x = 0; x < static_cast<s32>(typeGrid.sizeX()); ++x)
		{
			processTile(typeGrid, tileGrid, x, y);
		}
	}
}

void AutoTiler::processTile(const Array2D<Type> & typeGrid, Array2D<Tile> & tileGrid, u32 x, u32 y)
{
	// Get the type of the cell
	Type type = typeGrid.getNoEx(x,y);

	Tile tile = defaultTile;

	// If the type is referenced
	if(type < typeRules.size())
	{
		// If the type has no rules
		const TypeRules & rules = typeRules[type];
		if(rules.cases.empty())
		{
			// Apply default tile to the cell
			tile = rules.defaultTile;
		}
		else // The type has rules:
		{
			// Retrieve neighboring mask
			u32 m = 0;
			for(u32 i = 0; i < 8; ++i)
			{
				s32 nx = x + g_nv8[i][0];
				s32 ny = y + g_nv8[i][1];

				Type ntype = typeGrid.contains(nx, ny) ? typeGrid.getNoEx(nx, ny) : defaultType;

				u8 lookup = rules.defaultLookup;
				auto it = rules.lookups.find(ntype);
				if(it != rules.lookups.end())
				{
					lookup = it->second;
				}

				m |= lookup;

				if(i != 7)
				{
					m <<= 4;
				}
			}

			Neighboring neighboring = m;

			// Find a rule for the given neighboring
			auto ruleIt = rules.cases.find(neighboring);
			if(ruleIt != rules.cases.end())
			{
				// Found a rule, apply a corresponding tile
				tile = ruleIt->second[0]; // TODO choose variant at random
			}
			else
			{
				// No rules for this case, use type's default tile
				tile = rules.defaultTile;
			}
		}
	}

	// Place the tile
	tileGrid.setNoEx(x,y, tile);
}


// TODO JSON loader for AutoTiler

/*

{
	"defaultTile": 0,
	"typeRules": [
		{
			"defaultTile": 0,
			"cases": [
				{
					"n":[
						1, 1, 1,
						1,    1,
						0, 0, 0
					],
					"v":[ 3, 4 ]
				}
			]
		}
	]
}

*/

} // namespace zn

