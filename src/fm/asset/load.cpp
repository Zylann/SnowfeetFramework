#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fm/asset/Material.hpp>
#include <fm/asset/FileRef.hpp>
#include <fm/asset/TiledMap.hpp>
#include <fm/asset/TextureAtlas.hpp>

namespace zn
{

//------------------------------------------------------------------------------
bool loadFromFile(sf::Texture * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(sf::Shader * asset, const std::string & filePath)
{
	// Shaders are built from two files : vertex and fragment

	JsonBox::Value doc;
	if(!zn::loadFromFile(doc, filePath))
	{
		return false;
	}

	std::string folder = filePath.substr(0, filePath.find_last_of('/'));

	std::string fragmentPath = doc["fragment"].getString();
	std::string vertexPath = doc["vertex"].getString();

	if(vertexPath.empty() && fragmentPath.empty())
	{
		std::cout << "E: AssetMap<sf::Shader>::loadAsset: "
			"no shader specified" << std::endl;
		return false;
	}
	else if(!vertexPath.empty() && fragmentPath.empty())
	{
		return asset->loadFromFile(folder+'/'+vertexPath, sf::Shader::Vertex);
	}
	else if(vertexPath.empty() && !fragmentPath.empty())
	{
		return asset->loadFromFile(folder+'/'+fragmentPath, sf::Shader::Fragment);
	}
	else
	{
		return asset->loadFromFile(folder+'/'+vertexPath, folder+'/'+fragmentPath);
	}
}

//------------------------------------------------------------------------------
bool loadFromFile(sf::Font * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(TextureAtlas * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(TiledMap * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(sf::SoundBuffer * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(sf::Music * asset, const std::string & filePath)
{
	// Music is not really loaded, it is streamed
	return asset->openFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(FileRef * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

//------------------------------------------------------------------------------
bool loadFromFile(Material * asset, const std::string & filePath)
{
	return asset->loadFromFile(filePath);
}

} // namespace zn


