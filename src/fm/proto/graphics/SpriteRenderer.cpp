#include <iostream>
#include "SpriteRenderer.hpp"
#include "../Entity.hpp"
#include "../../sfml/sfml2_utils.hpp"
#include "../../json/json_utils.hpp"
#include "../../asset/AssetBank.hpp"

using namespace std;

namespace zn
{

//------------------------------------------------------------------------------
void SpriteRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

//------------------------------------------------------------------------------
void SpriteRenderer::setTextureRect(const sf::IntRect& rect)
{
	if(m_sprite.getTexture() == nullptr)
		return;
#ifdef ZN_DEBUG
	if(!checkTextureRect(*m_sprite.getTexture(), rect))
		cout << "E: SpriteRenderer::setTextureRect: invalid rectangle ("
			<< rect.left << ", " << rect.top << "; "
			<< rect.width << ", " << rect.height << ")" << endl;
#endif
	m_sprite.setTextureRect(rect);
}

//------------------------------------------------------------------------------
void SpriteRenderer::setAtlas(const TextureAtlas * atlas)
{
	if(entity().animator() != nullptr)
		entity().animator()->stop();
	r_atlas = atlas;
	m_sprite.setTexture(r_atlas->texture());
}

//------------------------------------------------------------------------------
void SpriteRenderer::setFrame(const std::string& id)
{
	if(r_atlas != nullptr)
	{
		const TextureAtlas::Frame * f = r_atlas->frame(id);
		if(f != nullptr)
		{
			m_sprite.setTextureRect(f->rect);

			if(entity().animator() != nullptr)
			{
				entity().animator()->stop();
			}
		}
#ifdef ZN_DEBUG
		else
			cout << "E: SpriteRenderer::setFrame: not found \"" << id << "\")" << endl;
#endif
	}
#ifdef ZN_DEBUG
	else
		cout << "E: SpriteRenderer::setFrame: no TextureAtlas" << endl;
#endif
}

//------------------------------------------------------------------------------
void SpriteRenderer::serializeData(JsonBox::Value & o)
{
	ARenderer::serializeData(o);

	zn::serialize(o["position"], position());
	zn::serialize(o["origin"], origin());
	zn::serialize(o["scale"], scale());
	o["rotation"] = rotation();

	zn::serialize(o["textureRect"], m_sprite.getTextureRect());

	if(r_atlas != nullptr)
	{
		std::string atlasName = AssetBank::current()->atlases.findName(r_atlas);
		o["atlas"] = atlasName;
	}
	else
	{
		const sf::Texture * texture = m_sprite.getTexture();
		std::string textureName = AssetBank::current()->textures.findName(texture);
		o["texture"] = textureName;
	}
}

//------------------------------------------------------------------------------
void SpriteRenderer::unserializeData(JsonBox::Value & o)
{
	ARenderer::unserialize(o);

	sf::Vector2f v;

	zn::unserialize(o["position"], v);   setPosition(v);
	zn::unserialize(o["origin"], v);     setOrigin(v);
	zn::unserialize(o["scale"], v);      setScale(v);

	setRotation(o["rotation"].getDouble());

	// Get atlas
	std::string atlasName = o["atlas"].getString();
	if(!atlasName.empty())
	{
		const TextureAtlas * atlas = AssetBank::current()->atlases.get(atlasName);
		setAtlas(atlas);
	}
	else // If there is no atlas, get simple texture
	{
		std::string textureName = o["texture"].getString();
		if(!textureName.empty())
		{
			const sf::Texture * texture = AssetBank::current()->textures.get(textureName);
			if(texture != nullptr)
			{
				setTexture(*texture);
			}
			else
			{
				std::cout << "E: SpriteRenderer::unserializeData: texture not found \"" << textureName << '"' << std::endl;
			}
		}
		else
		{
			std::cout << "E: SpriteRenderer::unserializeData: texture name is empty" << std::endl;
		}
	}

	// Set texture rect after the texture is being set
	sf::IntRect textureRect;
	zn::unserialize(o["textureRect"], textureRect);
	setTextureRect(textureRect);
}

//------------------------------------------------------------------------------
void SpriteRenderer::postUnserialize()
{

}

} // namespace zn


