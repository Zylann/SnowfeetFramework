#include <iostream>

#include <fm/proto/graphics/SpriteRenderer.hpp>
#include <fm/proto/core/Entity.hpp>
#include <fm/sfml/sfml2_utils.hpp>
#include <fm/asset/AssetBank.hpp>

#include <fm/json/json_utils.hpp>

using namespace std;

namespace zn
{

//------------------------------------------------------------------------------
SpriteRenderer::SpriteRenderer() : ARenderer(),
	r_texture(nullptr),
	r_atlas(nullptr)
{
	m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
	m_vertices.resize(4);
	m_textureRect = sf::IntRect(0,0,1,1);
	scaleToPixels(true);
}

//------------------------------------------------------------------------------
void SpriteRenderer::setTexture(const sf::Texture * texture)
{
	if(r_texture == nullptr && texture != nullptr)
	{
		setTextureRect(sf::IntRect(0,0, texture->getSize().x, texture->getSize().y));
	}
	r_texture = texture;
}

//------------------------------------------------------------------------------
void SpriteRenderer::setTexture(const sf::Texture * texture, bool resetRect)
{
	r_texture = texture;
	if(resetRect && r_texture != nullptr)
	{
		setTextureRect(sf::IntRect(0,0, texture->getSize().x, texture->getSize().y));
	}

	if(m_scaleToPixels)
		updateVertices();
}

//------------------------------------------------------------------------------
void SpriteRenderer::setColor(sf::Color color)
{
	m_vertices[0].color = color;
	m_vertices[1].color = color;
	m_vertices[2].color = color;
	m_vertices[3].color = color;
}

//------------------------------------------------------------------------------
const sf::Color & SpriteRenderer::color() const
{
	return m_vertices[0].color;
}

//------------------------------------------------------------------------------
void SpriteRenderer::setScale(const sf::Vector2f & scale)
{
	m_scale = scale;
	updateVertices();
}

//------------------------------------------------------------------------------
void SpriteRenderer::updateVertices()
{
	m_vertices[0].position = sf::Vector2f(0, 0);
	m_vertices[1].position = sf::Vector2f(m_scale.x, 0);
	m_vertices[2].position = sf::Vector2f(m_scale.x, m_scale.y);
	m_vertices[3].position = sf::Vector2f(0, m_scale.y);
}

//------------------------------------------------------------------------------
void SpriteRenderer::scaleToPixels(bool enable)
{
	m_scaleToPixels = enable;
	if(m_scaleToPixels)
	{
		setScale(sf::Vector2f(m_textureRect.width, m_textureRect.height));
	}
}

//------------------------------------------------------------------------------
void SpriteRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.combine(entity().transform.matrix());
	states.texture = r_texture;
	target.draw(m_vertices, states);
}

//------------------------------------------------------------------------------
sf::FloatRect SpriteRenderer::localBounds() const
{
	const sf::Vector2f & o = m_vertices[0].position;
	return sf::FloatRect(
		o.x,
		o.y,
		m_vertices[2].position.x - o.x,
		m_vertices[2].position.y - o.y
	);
}

//------------------------------------------------------------------------------
void SpriteRenderer::setTextureRect(const sf::IntRect& rect)
{
#ifdef ZN_DEBUG
	if(r_texture != nullptr)
	{
		if(!checkTextureRect(*r_texture, rect))
			log.warn() << "SpriteRenderer::setTextureRect: invalid rectangle ("
				<< rect.left << ", " << rect.top << "; "
				<< rect.width << ", " << rect.height << ")" << log.endl();
	}
#endif

	m_textureRect = rect;

	f32 left = rect.left;
	f32 right = rect.left + rect.width;
	f32 bottom = rect.top + rect.height;
	f32 top = rect.top;

	m_vertices[0].texCoords = sf::Vector2f(left, top);
	m_vertices[1].texCoords = sf::Vector2f(right, top);
	m_vertices[2].texCoords = sf::Vector2f(right, bottom);
	m_vertices[3].texCoords = sf::Vector2f(left, bottom);

	if(m_scaleToPixels)
	{
		setScale(sf::Vector2f(m_textureRect.width, m_textureRect.height));
	}
}

//------------------------------------------------------------------------------
void SpriteRenderer::setAtlas(const TextureAtlas * atlas)
{
	r_atlas = atlas;
	onAtlasChanged(atlas);
}

//------------------------------------------------------------------------------
void SpriteRenderer::setFrame(const std::string& id)
{
	if(r_atlas != nullptr)
	{
		const TextureAtlas::Frame * f = r_atlas->frame(id);
		if(f != nullptr)
		{
			setTextureRect(f->rect);

			if(entity().animator() != nullptr)
			{
				entity().animator()->stop();
			}
		}
#ifdef ZN_DEBUG
		else
			log.err() << "SpriteRenderer::setFrame: not found \"" << id << "\")" << log.endl();
#endif
	}
#ifdef ZN_DEBUG
	else
		log.err() << "SpriteRenderer::setFrame: no TextureAtlas" << log.endl();
#endif
}

//------------------------------------------------------------------------------
void SpriteRenderer::serializeData(JsonBox::Value & o)
{
	ARenderer::serializeData(o);

	zn::serialize(o["color"], color());
	zn::serialize(o["textureRect"], m_textureRect);
	zn::serialize(o["scale"], m_scale);
	o["scaleToPixels"] = m_scaleToPixels;

	std::string atlasName = AssetBank::current()->atlases.findName(r_atlas);
	o["atlas"] = atlasName;

	std::string textureName = AssetBank::current()->textures.findName(r_texture);
	o["texture"] = textureName;
}

//------------------------------------------------------------------------------
void SpriteRenderer::unserializeData(JsonBox::Value & o)
{
	ARenderer::unserialize(o);

	sf::Color c;
	zn::unserialize(o["color"], c);
	setColor(c);

	// Get atlas
	std::string atlasName = o["atlas"].getString();
	if(!atlasName.empty())
	{
		const TextureAtlas * atlas = AssetBank::current()->atlases.get(atlasName);
		setAtlas(atlas);
	}

	// Get texture
	std::string textureName = o["texture"].getString();
	if(!textureName.empty())
	{
		const sf::Texture * texture = AssetBank::current()->textures.get(textureName);
		if(texture != nullptr)
		{
			setTexture(texture);
		}
		else
		{
			log.err() << "SpriteRenderer::unserializeData: "
				"texture not found \"" << textureName << '"' << log.endl();
		}
	}
	else
	{
		log.err() << "SpriteRenderer::unserializeData: "
			"texture name is empty" << log.endl();
	}

	// Set texture rect after the texture is being set
	sf::IntRect textureRect;
	zn::unserialize(o["textureRect"], textureRect);
	zn::unserialize(o["scale"], m_scale);
	m_scaleToPixels = o["scaleToPixels"].getBoolean();
	setTextureRect(textureRect);

	updateVertices();
}

} // namespace zn


