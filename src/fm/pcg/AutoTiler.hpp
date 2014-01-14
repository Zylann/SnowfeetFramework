#ifndef HEADER_ZN_AUTOTILER_HPP_INCLUDED
#define HEADER_ZN_AUTOTILER_HPP_INCLUDED

#include <vector>
#include <array>
#include <map>
#include "fm/util/Array2D.hpp"

namespace zn
{

// Data-oriented auto-tiler.
// Given a grid of cell types, finds which tiles apply to them
// (handles connections, variants and default tiles)
class AutoTiler
{
public:

	typedef u32 Tile;
	typedef u8 Type;

	// 0 1 2
	// 3 x 4
	// 5 6 7
	typedef u64 Neighboring; // 01234567 (8 * 8bits)

	// Defines the rules applied to a type of tile depending on various neighborings,
	// in order to choose the good center tile from a series of possible variants
	struct TypeRules
	{
		Tile defaultTile;
		std::map<Neighboring, std::vector<Tile>> rules; // [neighbors][variant]

		void add(u8 v0, u8 v1, u8 v2, u8 v3, u8 v4, u8 v5, u8 v6, u8 v7, std::vector<Tile> variants)
		{
			Neighboring n = v0; n <<= 8;
			n |= v1; n <<= 8;
			n |= v2; n <<= 8;
			n |= v3; n <<= 8;
			n |= v4; n <<= 8;
			n |= v5; n <<= 8;
			n |= v6; n <<= 8;
			n |= v7;

			rules.insert(std::make_pair(n, variants));
		}
	};

	AutoTiler() :
		defaultTile(0),
		defaultType(0)
	{}

	Tile defaultTile;
	Type defaultType;

	std::vector<TypeRules> typeRules;

	void process(const Array2D<Type> typeGrid, Array2D<Tile> & tileGrid);

};

} // namespace zn


#endif // HEADER_ZN_AUTOTILER_HPP_INCLUDED




