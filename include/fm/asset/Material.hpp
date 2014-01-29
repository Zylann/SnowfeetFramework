#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include <fm/types.hpp>
#include <fm/json/json_utils.hpp>

namespace zn
{

// Holds informations about how to display vertices of a renderer such as
// the shader, its properties, the texture, the blending mode etc.
class ZN_API Material
{
public:

	// Shader standard parameter names (convention choice)
	//static const char * MAIN_TEXTURE_NAME;

	sf::Shader * shader;
	u32 blendMode;

	// Creates a material using the specified shader.
	Material(sf::Shader * pShader=nullptr) :
		shader(pShader),
		blendMode(sf::BlendMode::BlendAlpha)
	{}

	//-----------------------------
	// Parameter maps
	//-----------------------------

	template <typename T>
	using ParameterMap = std::unordered_map<std::string, T>;

	ParameterMap<f32> floatParams;
	ParameterMap<sf::Vector2f> vec2Params;
	ParameterMap<sf::Vector3f> vec3Params;
	ParameterMap<sf::Color> colorParams;
	ParameterMap<sf::Transform> mat4Params;
	ParameterMap<const sf::Texture*> tex2DParams;

	//-----------------------------
	// Application
	//-----------------------------

	// Applies the material to the given render states
	// (sets the blending mode and shader parameters)
	void apply(sf::RenderStates & renderStates);

	// Gets the main texture for this material, if any
	//sf::Texture * mainTexture() const;

	// Sets the main texture for this material
	// Note: it's a convention. It may be used or not depending on the shader.
	//void setMainTexture(sf::Texture * texture);

	//-----------------------------
	// Serialization
	//-----------------------------

	void serialize(JsonBox::Value & o) const;
	void unserialize(JsonBox::Value & o);

	bool loadFromFile(const std::string & filePath);

private:

	template <typename T>
	void applyParams(const ParameterMap<T> & params) const
	{
		for(auto it = params.cbegin(); it != params.cend(); ++it)
		{
			shader->setParameter(it->first, it->second);
		}
	}

	void applyTextureParams(const ParameterMap<const sf::Texture*> & params) const
	{
		for(auto it = params.cbegin(); it != params.cend(); ++it)
		{
			shader->setParameter(it->first, *(it->second));
		}
	}

	template <typename T>
	void serializeMap(JsonBox::Value & o, const ParameterMap<T> & params) const
	{
		for(auto it = params.cbegin(); it != params.cend(); ++it)
		{
			zn::serialize(o[it->first], it->second);
		}
	}

	// Specialization for textures because they shouldn't be serialized by value
	void serializeTextureMap(JsonBox::Value & o, const ParameterMap<const sf::Texture*> & params) const;

}; // class Material

} // namespace zn

#endif // MATERIAL_HPP_INCLUDED



