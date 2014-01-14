#ifndef HEADER_ZN_AUTOTILER_HPP_INCLUDED
#define HEADER_ZN_AUTOTILER_HPP_INCLUDED

#include <vector>
#include <map>
#include "fm/util/Array2D.hpp"

namespace zn
{

class AutoTiler
{
public:

	typedef u32 Tile;
	typedef u8 Type;
//	typedef u64 Neighboring;

	struct Neighboring
	{
		u32 upper;
		u32 lower;
	};

	// Defines the rules applied to a type of tile depending on various neighborings,
	// in order to choose the good center tile from a series of possible variants
	struct TypeRules
	{
		Tile defaultTile;
		std::map<Neighboring, std::vector<Tile>> rules; // [neighbors][variant]

//		void add(u64 v1, u64 v2, u64 v3, u64 v4,
//		         u64 v5, u64 v6, u64 v7, u64 v8,
		void add(u32 v1, u32 v2, u32 v3, u32 v4,
		         u32 v5, u32 v6, u32 v7, u32 v8,
		         std::vector<Tile> variants)
		{
//			Neighboring n = (v1 << 56) | (v2 << 48) | (v3 << 40) | (v4 << 32) |
//				            (v5 << 24) | (v6 << 16) | (v7 << 8) | v8 ;
			Neighboring n = { (v1 << 24) | (v2 << 16) | (v3 << 8) | v4,
				              (v5 << 24) | (v6 << 16) | (v7 << 8) | v8 };

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

bool operator<(const AutoTiler::Neighboring & a, const AutoTiler::Neighboring & b);

} // namespace zn


#endif // HEADER_ZN_AUTOTILER_HPP_INCLUDED




