#ifndef HEADER_ZN_GAMESETTINGS_HPP_INCLUDED
#define HEADER_ZN_GAMESETTINGS_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <fstream>

namespace zn
{

// Holds global GameApp settings such as window size, assets location, graphics settings...
struct GameSettings
{
	sf::VideoMode videoMode;
	std::string title;
	bool fullscreen;
	bool verticalSync;
	u32 antialias;
	std::string assetsRoot;
	bool showSystemCursor;

	// Constructs game settings with default values
	GameSettings() :
		videoMode(800, 600, 32),
		title("Game"),
		fullscreen(false),
		verticalSync(true),
		antialias(0),
		assetsRoot("assets"),
		showSystemCursor(true)
	{}

	bool loadFromJSONFile(const std::string & filePath)
	{
		JsonBox::Value v;
		std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
		if(!ifs.good())
		{
			return false;
		}
		v.loadFromStream(ifs);
		ifs.close();

		videoMode.width = v["width"].getInt();
		videoMode.height = v["height"].getInt();
		videoMode.bitsPerPixel = v["bitsPerPixel"].getInt();

		title = v["title"].getString();
		fullscreen = v["fullscreen"].getBoolean();
		verticalSync = v["verticalSync"].getBoolean();
		antialias = v["antialias"].getInt();
		showSystemCursor = v["showSystemCursor"].getBoolean();

		assetsRoot = v["assetsRoot"].getString();

		return true;
	}

	bool saveToJSONFile(const std::string & filePath)
	{
		JsonBox::Value v;

		v["width"] = (s32)videoMode.width;
		v["height"] = (s32)videoMode.height;
		v["bitsPerPixel"] = (s32)videoMode.bitsPerPixel;

		v["title"] = title;
		v["fullscreen"] = fullscreen;
		v["verticalSync"] = verticalSync;
		v["antialias"] = (s32)antialias;
		v["showSystemCursor"] = showSystemCursor;

		v["assetsRoot"] = assetsRoot;

		std::ofstream ofs(filePath.c_str(), std::ios::out|std::ios::trunc|std::ios::binary);
		if(!ofs.good())
		{
			std::cout << "E: couldn't save GameSettings to file \"" << filePath << '"' << std::endl;
			return false;
		}
		v.writeToStream(ofs);
		ofs.close();

		return false;
	}

};

} // namespace zn

#endif // ZN_GAMESETTINGS_HPP_INCLUDED
