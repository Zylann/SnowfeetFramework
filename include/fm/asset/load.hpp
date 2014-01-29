#ifndef HEADER_ZN_LOAD_HPP_INCLUDED
#define HEADER_ZN_LOAD_HPP_INCLUDED

namespace zn
{

// Asset loading overloads:
// They are required to not enforce function signature, especially for external classes.
// Note: this can't be done with template specialization, because it would require
// template code to be specialized in a CPP file (to avoid redefinition issues)
// and *likely* go in the engine's binary, wich is uneasy to link properly with user code

class TextureAtlas;
class TiledMap;
class FileRef;
class Material;

bool loadFromFile(sf::Texture * asset, const std::string & filePath);
bool loadFromFile(sf::Shader * asset, const std::string & filePath);
bool loadFromFile(sf::Font * asset, const std::string & filePath);
bool loadFromFile(TextureAtlas * asset, const std::string & filePath);
bool loadFromFile(TiledMap * asset, const std::string & filePath);
bool loadFromFile(sf::SoundBuffer * asset, const std::string & filePath);
bool loadFromFile(FileRef * asset, const std::string & filePath);
bool loadFromFile(Material * asset, const std::string & filePath);

} // namespace zn

#endif // HEADER_ZN_LOAD_HPP_INCLUDED


