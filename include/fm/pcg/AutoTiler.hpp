#ifndef HEADER_ZN_AUTOTILER_HPP_INCLUDED
#define HEADER_ZN_AUTOTILER_HPP_INCLUDED

#include <vector>
#include <array>
#include <unordered_map>
#include "fm/util/Array2D.hpp"

namespace zn
{

// Data-oriented auto-tiler.
// Given a grid where each cell is a type, finds which tiles apply to them
// (can handle connections, variants and default tiles)
class AutoTiler
{
public:

	typedef u32 Tile;
	typedef u8 Type;

	// Each value in this grid is a lookup refering to a type.
	// A lookup can refer up to 16 different types (4 bit values).
	// 0 1 2
	// 3 x 4
	// 5 6 7
	typedef u32 Neighboring; // 01234567 (8 * 4bits)

	// Defines the rules applied to a type of tile depending on various neighborings,
	// in order to choose the good center tile from a series of possible variants
	struct TypeRules
	{
		// Tile used if none of the rules match
		Tile defaultTile;

		// Relation between a specific Neighboring and the tiles that can be used
		std::unordered_map<Neighboring, std::vector<Tile>> cases; // [neighbors][variant]

		// Maps a type to its 4-bit lookup in Neighboring (kind of lightweight alias).
		// The use of lookups is up to the user.
		std::unordered_map<Type, u8> lookups;
		// If the type is not found in lookups when processing, this lookup is used.
		// Note: this converts unmapped types to a mapped one as fallback.
		// It also allows to define some kind of "any" type.
		u8 defaultLookup;

		// TODO add neighboring pattern option

		TypeRules():
			defaultTile(0),
			defaultLookup(0)
		{}

		// Sets a type to be seen as the given lookup in Neighboring keys
		void setLookup(u8 type, u8 lookup)
		{
#ifdef ZN_DEBUG
			if(lookup & 0xf0)
			{
				std::cout << "W: AutoTiler::TypeRules::setLookup: "
					"the given lookup is larger than 4 bits ! "
					"(type=" << (u32)type << ", lookup=" << (u32)lookup << ')'
					<< std::endl;
			}
#endif
			lookups[type] = lookup;
		}

		// Sets lookups so only cells of the same type will be seen as 1
		// and any other will be seen as 0
		void setLookupsAsOnly(u8 type)
		{
			defaultLookup = 0;
			lookups[type] = 1;
		}

		void addCase(u8 v0, u8 v1, u8 v2, u8 v3, u8 v4, u8 v5, u8 v6, u8 v7, std::vector<Tile> variants)
		{
			Neighboring n = v0; n <<= 4;
			n |= v1; n <<= 4;
			n |= v2; n <<= 4;
			n |= v3; n <<= 4;
			n |= v4; n <<= 4;
			n |= v5; n <<= 4;
			n |= v6; n <<= 4;
			n |= v7;

			cases.insert(std::make_pair(n, variants));
		}
	};

	AutoTiler() :
		defaultTile(0),
		defaultType(0)
	{}

	Tile defaultTile; // Tile used when no rules match
	Type defaultType; // Type used when out of bounds

	// Tiling rules for each type in input grids
	std::vector<TypeRules> typeRules;

	// Converts a grid of types into a grid of tiles
	void process(const Array2D<Type> & typeGrid, Array2D<Tile> & tileGrid);

	// Calculates a tile from its type at the given position
	void processTile(const Array2D<Type> & typeGrid, Array2D<Tile> & tileGrid, u32 x, u32 y);

};

} // namespace zn


#endif // HEADER_ZN_AUTOTILER_HPP_INCLUDED




