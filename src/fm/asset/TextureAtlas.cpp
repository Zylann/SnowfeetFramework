/*
TextureAtlas.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include "TextureAtlas.hpp"
#include "../rapidjson/document.h"
#include "../rapidjson/filestream.h"
#include "../sfml/sfml2_utils.hpp"

using namespace rapidjson;

namespace zn
{

TextureAtlas::TextureAtlas()
{
}

//------------------------------------------------------------------------------
bool TextureAtlas::loadFromFile(const std::string & filePath)
{
	return loadFromJSONFile(filePath);
}

//------------------------------------------------------------------------------
bool TextureAtlas::loadFromJSONFile(const std::string & filePath)
{
	FILE * cfile = fopen(filePath.c_str(), "rb");
	if(!cfile)
	{
		std::cout << "E: TiledMap::loadFromJSONFile: couldn't open \"" + filePath + '"' << std::endl;
		return false;
	}

	std::cout << "Reading TextureAtlas..." << std::endl;

	// Parse stream

	FileStream fs(cfile);
	Document doc;
	doc.ParseStream<0>(fs);

	// Check version

	assert(doc.IsObject());
	assert(doc.HasMember("version"));

	s32 version = doc["version"].GetInt();
	if(version != 1)
	{
		std::cout << "E: TextureAtlas: unsupported JSON version (" << version << ")" << std::endl;
		fclose(cfile);
		return false;
	}

	// Globals

	std::string textureName = doc["texture"].GetString();
	s32 prescale = doc["prescale"].GetInt();
	s32 timescale = doc["timescale"].GetInt();

	// Frames

	const Value & jframes = doc["frames"];
	for(SizeType i = 0; i < jframes.Size(); ++i)
	{
		Frame f;

		std::string name = jframes[i]["name"].GetString();

		f.rect.left = prescale * jframes[i]["x"].GetInt();
		f.rect.top = prescale * jframes[i]["y"].GetInt();
		f.rect.width = prescale * jframes[i]["w"].GetInt();
		f.rect.height = prescale * jframes[i]["h"].GetInt();

		if(m_frames.find(name) != m_frames.end())
		{
			std::cout << "W: TextureAtlas: found duplicate frame \"" << name << "\" in " << filePath << std::endl;
		}

		m_frames[name] = f;
	}

	// Sequences

	const Value & jsequences = doc["sequences"];
	for(SizeType i = 0; i < jsequences.Size(); ++i)
	{
		Sequence seq;

		std::string name = jsequences[i]["name"].GetString();

		const Value & jframes = jsequences[i]["frames"];
		for(SizeType i = 0; i < jframes.Size(); ++i)
		{
			SequenceFrame f;

			f.rect.left = prescale * jframes[i]["x"].GetInt();
			f.rect.top = prescale * jframes[i]["y"].GetInt();
			f.rect.width = prescale * jframes[i]["w"].GetInt();
			f.rect.height = prescale * jframes[i]["h"].GetInt();

			f.duration = timescale * jframes[i]["d"].GetInt();

			seq.frames.push_back(f);
		}

		if(m_sequences.find(name) != m_sequences.end())
		{
			std::cout << "W: TextureAtlas: found duplicate sequence \"" << name << "\" in " << filePath << std::endl;
		}

		m_sequences[name] = seq;
	}

	fclose(cfile);

	std::string localDir = filePath.substr(0, filePath.find_last_of("/\\"));
	std::string texturePath = localDir + '/' + textureName;
	if(!m_texture.loadFromFile(texturePath))
	{
		std::cout << "E: TextureAtlas: couldn't load texture " << texturePath << std::endl;
		return false;
	}

	m_texture.setSmooth(false);

	return true;
}

//------------------------------------------------------------------------------
const TextureAtlas::Frame * TextureAtlas::frame(std::string id) const
{
	auto it = m_frames.find(id);
	if(it != m_frames.end())
		return &it->second;
	else
		return nullptr;
}

//------------------------------------------------------------------------------
const TextureAtlas::Sequence * TextureAtlas::sequence(std::string id) const
{
	auto it = m_sequences.find(id);
	if(it != m_sequences.end())
		return &it->second;
	else
		return nullptr;
}

//------------------------------------------------------------------------------
void TextureAtlas::addFrame(std::string id, sf::IntRect rect)
{
#ifdef ZN_DEBUG
	if(frame(id) != nullptr)
		std::cout << "W: TextureAtlas::addFrame: overwrite previous ID (" << id << std::endl;
#endif
	m_frames[id].rect = rect;
}

//------------------------------------------------------------------------------
void TextureAtlas::addSequence(std::string id, const Sequence & seq)
{
#ifdef ZN_DEBUG
	if(sequence(id) != nullptr)
		std::cout << "W: TextureAtlas::addFrame: overwrite previous ID (" << id << std::endl;
#endif
	m_sequences[id] = seq;
}

//------------------------------------------------------------------------------
bool TextureAtlas::checkFrame(const sf::IntRect & rect)
{
	//cout << " " << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << " ";
	return checkTextureRect(m_texture, rect);
}

//------------------------------------------------------------------------------
const sf::Vector2i TextureAtlas::frameFromNumber(u32 i, const sf::Vector2i & tileSize) const
{
	return sf::Vector2i(
		i*tileSize.x % m_texture.getSize().x,
		i/(m_texture.getSize().x / tileSize.x) * tileSize.y
	);
}

} // namespace zn

