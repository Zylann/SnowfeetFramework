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

/// \brief Loads a shader from a file containing both vertex and fragment programs.
/// Line positions are preserved for easier compilation error fixes.
/// ---------------
/// Syntax:
/// #vertex
/// <put here all the usual vertex shader code>
/// #fragment
/// <put here all the usual fragment shader code>
/// ---------------
/// \param shader: shader object to load
/// \param filePath: path to the shader file
/// \return true if success, false if an error occurred (the shader is untouched if so).
/// \note The required syntax is arbitrary and doesn't comes from any official specification.
bool loadMergedShaderFromFile(sf::Shader & shader, const std::string & filePath);

} // namespace zn

#endif // HEADER_ZN_LOAD_HPP_INCLUDED


