#include <cstdio>
#include "fm/util/PathFinder.hpp"
#include "test.hpp"

using namespace std;

namespace zn
{

void test_pathfinder()
{
	u32 width = 20;
	u32 height = 10;

	string mapStr =
	"XXXXX               "
	"XXXX    X           "
	"        X     XX    "
	"        X    X      "
	"    XXXXXXXXXX      "
	"         X          "
	"      X             "
	"      XXXXXXXXX     "
	"XXX          XXXXX X"
	"XXXXX         XXXXXX";

	if(mapStr.size() != width * height)
	{
		cout << "E: inconsistent test map" << endl;
		return;
	}

	s8 map[width*height];

	// Note: indexing is commonly done by x+y*width

	for(u32 i = 0; i < mapStr.size(); ++i)
	{
		if(mapStr[i] == 'X')
		{
			map[i] = 0;
		}
		else
		{
			map[i] = 1;
		}
	}

	s32 startX = 11;
	s32 startY = 3;
	s32 endX = 11;
	s32 endY = 8;

	PathFinder pathfinder(map, width, height);
	vector<PathFinder::Node> * path = pathfinder.findPath(startX, startY, endX, endY);

	if(path == nullptr)
	{
		cout << "Path not found" << endl;
		getchar();
		return;
	}

	for(u32 i = 0; i < path->size(); ++i)
	{
		PathFinder::Node n = (*path)[i];
		u32 l = n.x + n.y * width;
		mapStr[l] = 'o';
	}

	u32 i = 0;
	for(u32 y = 0; y < height; ++y)
	{
		for(u32 x = 0; x < width; ++x)
		{
			cout << mapStr[i];
			++i;
		}

		cout << '\n';
	}

	getchar();

	return;
}

} // namespace zn


