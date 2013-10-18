#include <iostream>
#include "test_json.hpp"
#include "fm/rapidjson/document.h"
#include "fm/rapidjson/filestream.h"

using namespace rapidjson;

void test_json()
{
	FILE * cfile = fopen("src/test/room_atlas.json", "rb");
	if(!cfile)
	{
		std::cout << "E: failed to open file" << std::endl;
		return;
	}

	std::cout << "Reading..." << std::endl;

	FileStream fs(cfile);
	Document doc;
	doc.ParseStream<0>(fs);

	assert(doc.IsObject());
	doc.HasMember("version");

	fclose(cfile);

	std::cout << "Done" << std::endl;

	getchar();
}


