#include <iostream>
#include "test.hpp"
#include "fm/rapidjson/document.h"
#include "fm/rapidjson/filestream.h"

using namespace rapidjson;

void test_json()
{
	//
	// Parse
	//

	FILE * cfile = fopen("test_data/room_atlas.json", "rb");
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

	//
	// Write
	//

	// TODO test rapidjson's writing API
}


