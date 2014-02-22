#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <fm/asset/Material.hpp>
#include <fm/asset/FileRef.hpp>
#include <fm/asset/TiledMap.hpp>
#include <fm/asset/TextureAtlas.hpp>
#include <fm/asset/load.hpp>
#include <fm/util/Log.hpp>

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
	std::string ext = filePath.substr(filePath.find_last_of('.'));

	if(ext == ".json")
	{
		// Load the shader from a JSON descriptor

		JsonBox::Value doc;
		if(!zn::loadFromFile(doc, filePath))
		{
			return false;
		}

		std::string folder = filePath.substr(0, filePath.find_last_of('/'));

		std::string fragmentPath = doc["fragment"].getString();
		std::string vertexPath = doc["vertex"].getString();

		// Shaders are built from two files : vertex and fragment

		if(vertexPath.empty() && fragmentPath.empty())
		{
			log.err() << "Cannot load shader from JSON descriptor: "
				"no shader specified in \"" << filePath << '"' << log.endl();
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
	else if(ext == ".shader")
	{
		// Load the shader from a merged file
		return loadMergedShaderFromFile(*asset, filePath);
	}
	else
	{
		log.err() << "Cannot load shader file, the \"" << ext << "\" file extension is not supported." << log.endl();
#ifdef ZN_DEBUG
		log.err() << "| use a JSON descriptor or a merged file to properly load the vertex+fragment programs." << log.endl();
#endif
		return false;
	}

	return true;
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

//------------------------------------------------------------------------------
bool loadMergedShaderFromFile(sf::Shader & shader, const std::string & filePath)
{
	std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
	if(!ifs.good())
	{
		log.err() << "Couldn't open merged shader file \"" << filePath << '"' << log.endl();
		return false;
	}

	const unsigned int VERTEX = 0;
	const unsigned int FRAGMENT = 1;
	const unsigned int NO_MODE = 2;

	std::string shaderStr[2];

	unsigned int mode = NO_MODE;

	const std::string vertexCommand = "#vertex";
	const std::string fragmentCommand = "#fragment";

	while(!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);
		line += '\n';

		if(!line.empty() && line[0] == '#')
		{
			if(line.find(vertexCommand) != std::string::npos)
			{
				// Treat next lines as a vertex program
				mode = VERTEX;
				shaderStr[0] += '\n';
				shaderStr[1] += '\n';
				continue;
			}
			else if(line.find(fragmentCommand) != std::string::npos)
			{
				// Treat next lines as a fragment program
				mode = FRAGMENT;
				shaderStr[0] += '\n';
				shaderStr[1] += '\n';
				continue;
			}
		}

		// Append code to the right shader, and append
		// newlines in the other to keep line numbers in sync with the
		// merged file
		switch(mode)
		{
		case VERTEX:
			shaderStr[VERTEX] += line;
			shaderStr[FRAGMENT] += '\n';
			break;

		case FRAGMENT:
			shaderStr[VERTEX] += '\n';
			shaderStr[FRAGMENT] += line;
			break;

		default:
			shaderStr[VERTEX] += '\n';
			shaderStr[FRAGMENT] += '\n';
			break;
		}
	}

	ifs.close();

	if(!shader.loadFromMemory(shaderStr[VERTEX], shaderStr[FRAGMENT]))
	{
		log.err() << "An error occurred reading merged shader file "
			"\"" << filePath << '"' << log.endl();
		return false;
	}

	return true;
}


} // namespace zn


