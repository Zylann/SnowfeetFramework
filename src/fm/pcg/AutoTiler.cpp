#include "AutoTiler.hpp"

namespace zn
{

void AutoTiler::process(const Array2D<Type> typeGrid, Array2D<Tile> & tileGrid)
{
	// 8-connexity neighboring vectors from center
	const s32 nvx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	const s32 nvy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	// For each cell in the grid
	for(s32 y = 0; y < static_cast<s32>(typeGrid.sizeY()); ++y)
	{
		for(s32 x = 0; x < static_cast<s32>(typeGrid.sizeX()); ++x)
		{
			// Get the type of the cell
			Type type = typeGrid.getNoEx(x,y);

			Tile tile = defaultTile;

			// If the type is referenced
			if(type < typeRules.size())
			{
				// If the type has no rules
				const TypeRules & tr = typeRules[type];
				if(tr.rules.empty())
				{
					// Apply default tile to the cell
					tile = tr.defaultTile;
				}
				else // The type has rules:
				{
					// Retrieve neighboring mask
					u64 m = 0;
					for(u32 i = 0; i < 8; ++i)
					{
						s32 nx = x + nvx[i];
						s32 ny = y + nvy[i];

						Type ntype = typeGrid.contains(nx, ny) ? typeGrid.getNoEx(nx, ny) : defaultType;

						m |= ntype;

						if(i != 7)
						{
							m <<= 8;
						}
					}

					Neighboring neighboring = m;

					// Find a rule for the given neighboring
					auto ruleIt = tr.rules.find(neighboring);
					if(ruleIt != tr.rules.end())
					{
						// Found a rule, apply a corresponding tile
						tile = ruleIt->second[0]; // TODO choose variant at random
					}
					else
					{
						// No rules for this case, use type's default tile
						tile = tr.defaultTile;
					}
				}
			}

			// Place the tile
			tileGrid.setNoEx(x,y, tile);
		}
	}
}

// TODO JSON loader for AutoTiler

/*

{
	"defaultTile": 0,
	"typeRules": [
		{
			"defaultTile": 0,
			"rules": [
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

