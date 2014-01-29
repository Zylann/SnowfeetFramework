#include <fm/asset/Material.hpp>
#include <fm/asset/AssetBank.hpp>
#include <fm/json/json_utils.hpp>

namespace zn
{

//const char * Material::MAIN_TEXTURE_NAME = "texture";

// TODO Materials are currently a bit limited by the way SFML handles shaders.
// in the future it would be nice to integrate a more custom implementation.

//------------------------------------------------------------------------------
u32 stringToBlendMode(const std::string str)
{
	if(str == "alpha")
	{
		return sf::BlendMode::BlendAlpha;
	}
	else if(str == "multiply")
	{
		return sf::BlendMode::BlendMultiply;
	}
	else if(str == "add")
	{
		return sf::BlendMode::BlendAdd;
	}
	else// if(str == "none")
	{
		return sf::BlendMode::BlendNone;
	}
}

//------------------------------------------------------------------------------
std::string blendModeToString(u32 blendMode)
{
	switch(blendMode)
	{
	case sf::BlendMode::BlendAlpha: return "alpha";
	case sf::BlendMode::BlendAdd: return "add";
	case sf::BlendMode::BlendMultiply: return "multiply";
	default: return "none";
	}
}

//------------------------------------------------------------------------------
void Material::apply(sf::RenderStates & renderStates)
{
	renderStates.shader = shader;
	renderStates.blendMode = (sf::BlendMode)blendMode;

	if(shader != nullptr)
	{
		applyParams(floatParams);
		applyParams(vec2Params);
		applyParams(vec3Params);
		applyParams(colorParams);
		applyParams(mat4Params);
		applyTextureParams(tex2DParams);
	}
}

//------------------------------------------------------------------------------
void Material::serialize(JsonBox::Value & o) const
{
	// Parameters
	JsonBox::Value & paramsData = o["parameters"];
	serializeMap(paramsData["float"], floatParams);
	serializeMap(paramsData["vec2"], vec2Params);
	serializeMap(paramsData["vec3"], vec3Params);
	serializeMap(paramsData["color"], colorParams);
	serializeMap(paramsData["mat4"], mat4Params);
	serializeTextureMap(paramsData["tex2D"], tex2DParams);

	// Blend mode
	o["blend"] = blendModeToString(blendMode); // to be readable !

	// Shader
	if(shader == nullptr)
	{
		o["shader"].setNull();
	}
	else
	{
		std::string shaderName = AssetBank::current()->shaders.findName(shader);
		if(!shaderName.empty())
		{
			o["shader"] = shaderName;
		}
#ifdef ZN_DEBUG
		else
		{
			std::cout << "D: Material::serialize: "
				"shader is not an asset, it will not be serialized" << std::endl;
		}
#endif
	}
}

//------------------------------------------------------------------------------
void Material::serializeTextureMap(JsonBox::Value & o, const ParameterMap<const sf::Texture*> & params) const
{
	for(auto it = params.cbegin(); it != params.cend(); ++it)
	{
		std::string textureName = AssetBank::current()->textures.findName(it->second);
		if(!textureName.empty())
		{
			o[it->first] = textureName;
		}
#ifdef ZN_DEBUG
		else
		{
			std::cout << "D: Material::serializeMap: "
				"texture parameter \"" << it->first << "\" is not an asset, "
				"it will not be serialized" << std::endl;
		}
#endif
	}
}

//------------------------------------------------------------------------------
void Material::unserialize(JsonBox::Value & o)
{
	// Blend mode

	blendMode = stringToBlendMode(o["blend"].getString());

	// Shader

	std::string shaderName = o["shader"].getString();
	if(!shaderName.empty())
	{
		shader = AssetBank::current()->shaders.get(shaderName);
#ifdef ZN_DEBUG
		if(shader == nullptr)
		{
			std::cout << "E: Material::unserialize: "
				"shader not found \"" << shaderName << "\"" << std::endl;
		}
#endif
	}

	// Parameters

	JsonBox::Value & paramsDataV = o["parameters"];
	const JsonBox::Object & paramsData = paramsDataV.getObject();

	// For each parameter
	for(JsonBox::Object::const_iterator it = paramsData.begin(); it != paramsData.end(); ++it)
	{
		const JsonBox::Value & v = it->second;

		if(v.isDouble())
		{
			// Single number
			floatParams[it->first] = v.getDouble();
		}
		else if(v.isString())
		{
			// Reference to a texture
			const std::string & textureName = v.getString();
			const sf::Texture * texture = AssetBank::current()->textures.get(textureName);
#ifdef ZN_DEBUG
			if(texture == nullptr)
			{
				std::cout << "E: Material::unserialize: "
					"texture not found \"" << textureName << "\""
					"(parameter: \"" << it->first << "\")" << std::endl;
			}
#endif
			tex2DParams[it->first] = texture;
		}
		else if(v.isArray())
		{
			// Array-form of a vector or matrix

			const JsonBox::Array & a = v.getArray();

			switch(v.getArray().size())
			{
			case 2:
				vec2Params[it->first] = sf::Vector2f(
					a[(size_t)0].getDouble(),
					a[1].getDouble()
				);
				break;

			case 3:
				vec3Params[it->first] = sf::Vector3f(
					a[0].getDouble(),
					a[1].getDouble(),
					a[2].getDouble()
				);
				break;

			case 4:
				colorParams[it->first] = sf::Color(
					a[0].getInt(),
					a[1].getInt(),
					a[2].getInt(),
					a[3].getInt()
				);
				break;

			case 16:
				zn::unserialize(v, mat4Params[it->first]);
				break;

			default:
#ifdef ZN_DEBUG
				std::cout << "W: Material::unserialize: "
					"unsupported array parameter size (" << a.size() << ")" << std::endl;
#endif
				break;
			}
		}
		else if(v.isObject())
		{
			// Vector or color

			JsonBox::Object o = v.getObject();

			if(!o["z"].isNull())
			{
				vec3Params[it->first] = sf::Vector3f(
					o["x"].getDouble(),
					o["y"].getDouble(),
					o["z"].getDouble()
				);
			}
			else if(!o["y"].isNull())
			{
				vec2Params[it->first] = sf::Vector2f(
					o["x"].getDouble(),
					o["y"].getDouble()
				);
			}
			else if(!o["r"].isNull())
			{
				colorParams[it->first] = sf::Color(
					o["r"].getInt(),
					o["g"].getInt(),
					o["b"].getInt(),
					o["a"].getInt()
				);
			}
#ifdef ZN_DEBUG
			else
			{
				std::cout << "W: Material::unserialize: "
					"unsupported object parameter \"" << it->first << '"' << std::endl;
			}
#endif
		}
	}

	blendMode = (u32)(o["blend"].getInt());
}

//------------------------------------------------------------------------------
bool Material::loadFromFile(const std::string & filePath)
{
	JsonBox::Value doc;
	if(!zn::loadFromFile(doc, filePath))
	{
		std::cout << "E: Material::loadFromFile: error while reading the file." << std::endl;
		return false;
	}

	unserialize(doc);

	return true;
}

//------------------------------------------------------------------------------
/*
sf::Texture * Material::mainTexture() const
{
	auto it = tex2DParams.find(MAIN_TEXTURE_NAME);
	if(it == tex2DParams.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}
*/
//------------------------------------------------------------------------------
/*
void Material::setMainTexture(sf::Texture * texture)
{
#ifdef ZN_DEBUG
	if(tex2DParams.find(MAIN_TEXTURE_NAME) == tex2DParams.end())
	{
		std::cout << "W: Material::setMainTexture: "
			"parameter \"" << MAIN_TEXTURE_NAME << "\" not found" << std::endl;
	}
#endif
	tex2DParams[MAIN_TEXTURE_NAME] = texture;
}
*/

} // namespace zn





