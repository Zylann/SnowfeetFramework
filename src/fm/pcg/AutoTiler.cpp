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

//------------------------------------------------------------------------------
void AutoTiler::RuleSet::addCase(ConnectionMask neighboring, u8 mask, std::vector<Out_T> variants)
{
	// Is the case fully-specific?
	if(mask == 0xf)
	{
		// The mask is full, no need to generate cases
		addCase(neighboring, variants);
		return;
	}

	// Is the mask "don't care at all"?
	if(mask == 0x0)
	{
		// It means we don't even need to watch for neighboring,
		// just use the defaultOuput
		defaultOutput = variants;
		return;
	}

	// mask is neither full nor empty, then some neighbors are "don't care".
	// Generate all cases for these locations
	// (maximum combination count is 256, but it's the case handled by
	// defaultOutput already, so a real count would be 128, which is not so big).

	// Ensure "don't care" neighbors are unset (these will be generated)
	neighboring &= mask;

	// Count zero bits in the mask and memorize their position
	u8 bits[8]; // 1-bit masks representing the Nth variable bit
	u8 bitCount = 0;
	for(u8 i = 0; i < 8; ++i)
	{
		u8 bit = (1 << i);
		if(!(mask & bit))
		{
			bits[bitCount++] = bit;
		}
	}

	// TODO test case generation

	// Generate possible neighborings :
	// Use a number going from 0 to max, and dispatch its bits in the final neighboring where bits are variable.
	//---------
	// Example
	//---------
	// Some case:           0b10100100
	//                          +
	// Mask:                0b11100101
	//                          =
	// Means:               0b101**1*0
	//
	// Needed combinations: 0b10100100  n = 0b000 (0)
	//                           ^^ ^
	//                      0b10100110  n = 0b001 (1)
	//                           ^^ ^
	//                      0b10101100  n = 0b010 (2)
	//                           ^^ ^
	//                      0b10101110  n = 0b011 (3)
	//                      ...
	//                      0b10111110  n = 0b111 (7)
	//                           ^^ ^
	u16 max = 1 << bitCount;
	for(u16 n = 0; n < max; ++n)
	{
		u8 generatedNeighboring = neighboring;
		for(u8 i = 0; i < bitCount; ++i)
		{
			u8 bit = (1 << i);
			if(n & bit)
			{
				generatedNeighboring |= bits[i];
			}
		}

		addCase(generatedNeighboring, variants);
	}
}

//------------------------------------------------------------------------------
void AutoTiler::process(const Array2D<In_T> & inputGrid, Array2D<Out_T> & outputGrid)
{
	// For each cell in the grid
	for(s32 y = 0; y < static_cast<s32>(inputGrid.sizeY()); ++y)
	{
		for(s32 x = 0; x < static_cast<s32>(inputGrid.sizeX()); ++x)
		{
			outputGrid.set(x,y, processTile(inputGrid, x, y));
		}
	}
}

//------------------------------------------------------------------------------
AutoTiler::Out_T AutoTiler::processTile(const Array2D<In_T> & inputGrid, u32 x, u32 y)
{
	// Get the type of the cell
	In_T type = inputGrid.getNoEx(x,y);

	Out_T outputValue = defaultOutput;

	// If the type is referenced
	if(type < ruleSets.size())
	{
		// If the type has no rules
		const RuleSet & rules = ruleSets[type];
		if(rules.cases.empty())
		{
			// Apply default outputValue to the cell
			outputValue = rules.defaultOutput[0];
		}
		else // The type has rules:
		{
			// Retrieve neighboring mask
			u8 m = 0;
			for(u32 i = 0; i < 8; ++i)
			{
				s32 nx = x + g_nv8[i][0];
				s32 ny = y + g_nv8[i][1];

				// Get neighboring value
				In_T ntype = inputGrid.contains(nx, ny) ? inputGrid.getNoEx(nx, ny) : defaultInput;

				// If it connects
				if(rules.connections.find(ntype) != rules.connections.end())
				{
					// Add to the mask
					m |= 1;
				}

				if(i != 7)
				{
					// Shift the mask to enter next value on next loop
					m <<= 1;
				}
			}

			ConnectionMask neighboring = m;

			// Find a rule for the given neighboring
			auto ruleIt = rules.cases.find(neighboring);
			if(ruleIt != rules.cases.end())
			{
				// Found a rule, apply a corresponding outputValue
				outputValue = ruleIt->second[0]; // TODO choose variant at random
			}
			else
			{
				// No rules for this case, use type's default outputValue
				outputValue = rules.defaultOutput[0];
			}
		}
	}

	return outputValue;
}


// TODO JSON loader for AutoTiler

/*

{
	"defaultOutput": 0,
	"typeRules": [
		{
			"defaultOutput": 0,
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

