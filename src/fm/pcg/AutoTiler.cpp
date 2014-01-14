#include "AutoTiler.hpp"

namespace zn
{

bool operator<(const AutoTiler::Neighboring & a, const AutoTiler::Neighboring & b)
{
	if(a.lower == b.lower)
		return a.upper < b.upper;
	return a.lower < b.lower;
}

void AutoTiler::process(const Array2D<Type> typeGrid, Array2D<Tile> & tileGrid)
{
	const s32 nvx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	const s32 nvy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	for(s32 y = 0; y < static_cast<s32>(typeGrid.sizeY()); ++y)
	{
		for(s32 x = 0; x < static_cast<s32>(typeGrid.sizeX()); ++x)
		{
			Type type = typeGrid.getNoEx(x,y);
			Tile tile = defaultTile;

			if(type < typeRules.size())
			{
				const TypeRules & tr = typeRules[type];

				if(tr.rules.empty())
				{
					tile = tr.defaultTile;
				}
				else
				{
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

					Neighboring n = { m >> 32, m & 0xffffffff };
					//std::cout << n.upper << '|' << n.lower << std::endl;

					auto ruleIt = tr.rules.find(n);
					if(ruleIt != tr.rules.end())
					{
						tile = ruleIt->second[0]; // TODO choose variant at random
					}
					else
					{
						tile = tr.defaultTile;
					}
				}
			}

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

