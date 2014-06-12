#include <fm/sfml/sfml2_utils.hpp>
#include <fm/util/math.hpp>
#include <fm/asset/AssetBank.hpp>
#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/Scene.hpp>
#include <fm/scene/graphics/ParticleSystem.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

ZN_OBJECT_IMPL(zn::ParticleSystem)

//------------------------------------------------------------------------------
void ParticleSystem::setAtlas(const TextureAtlas* atlas)
{
	r_atlas = atlas;
}

//------------------------------------------------------------------------------
void ParticleSystem::setTexture(const sf::Texture * texture)
{
	r_texture = texture;
}

//------------------------------------------------------------------------------
void ParticleSystem::setTextureRect(const sf::IntRect& rect)
{
	if(r_texture == nullptr)
		return;
#ifdef ZN_DEBUG
	if(!checkTextureRect(*r_texture, rect))
		log.err() << "ParticleSystem::setTextureRect: invalid rectangle ("
			<< rect.left << ", " << rect.top << "; "
			<< rect.width << ", " << rect.height << ")" << log.endl();
#endif
	m_atlasRect = rect;
}

//------------------------------------------------------------------------------
sf::FloatRect ParticleSystem::localBounds() const
{
	return m_emissionZone;
}

//------------------------------------------------------------------------------
void ParticleSystem::setMaxParticles(u32 max)
{
	if(m_maxParticles != max)
	{
		if(max == 0)
		{
			m_particles.clear();
			m_vertices.clear();
		}
		else
		{
			// TODO wait until enough particles fade

			m_particles.resize(max);
			m_vertices.resize(4*max);

			if(max > m_maxParticles)
			{
				updateUVs(m_maxParticles);
			}
		}

		m_maxParticles = max;
	}
}

//------------------------------------------------------------------------------
void ParticleSystem::updateUVs(u32 i_start)
{
	if(r_texture == nullptr)
	{
		log.err() << "ParticleSystem::updateUVs: no atlas defined !" << log.endl();
		return;
	}

	f32 minX = m_atlasRect.left;
	f32 minY = m_atlasRect.top;
	f32 maxX = m_atlasRect.left+m_atlasRect.width-1;
	f32 maxY = m_atlasRect.top+m_atlasRect.height-1;

	for(u32 i = i_start; i < m_particles.size(); ++i)
	{
		// Set UVs counter-clockwise
		sf::Vertex * quad = &m_vertices[i*4];
		quad[0].texCoords = sf::Vector2f(minX, minY);
		quad[1].texCoords = sf::Vector2f(maxX, minY);
		quad[2].texCoords = sf::Vector2f(maxX, maxY);
		quad[3].texCoords = sf::Vector2f(minX, maxY);
	}
}

//------------------------------------------------------------------------------
void ParticleSystem::setParticleRadius(f32 min, f32 max)
{
	m_particleRadius.set(min, max);
}

//------------------------------------------------------------------------------
void ParticleSystem::setParticleLifeTime(f32 min, f32 max)
{
	m_particleLifeTime.set(min, max);
}

//------------------------------------------------------------------------------
// TODO more emission zone types
void ParticleSystem::setEmissionZone(const sf::FloatRect& rect)
{
	m_emissionZone = rect;
}

//------------------------------------------------------------------------------
void ParticleSystem::onCreate()
{
}

//------------------------------------------------------------------------------
void ParticleSystem::onUpdate()
{
	//sf::Vector2f ownerPos = entity().position();

	for(u32 i = 0; i < m_particles.size(); ++i)
	{
		Particle & p = m_particles[i];

		if(p.remainingTime < 0)
		{
			p.remainingTime = math::randf(m_particleLifeTime.min(), m_particleLifeTime.max());

			p.pos.x = math::randf(m_emissionZone.left, m_emissionZone.left+m_emissionZone.width);
			p.pos.y = math::randf(m_emissionZone.top, m_emissionZone.top+m_emissionZone.height);

			// TODO customize velocity
			p.velocity.x = math::randf(-0.5f, 0.5f);
			p.velocity.y = math::randf(-0.5f, 0.5f);

			p.radius = math::randf(m_particleRadius.min(), m_particleRadius.max());
		}
		else
		{
			// TODO
			p.remainingTime -= entity().scene().deltaTime().asSeconds();
			p.pos += p.velocity;
		}

		// Update mesh quads

		u32 quad = i*4;
		sf::Vector2f pos(p.pos.x - 0.5f*p.radius, p.pos.y - 0.5f*p.radius);
		// Counter-clockwise quad
		                   m_vertices[quad+1].position = pos;
		pos.x += p.radius; m_vertices[quad+2].position = pos;
		pos.y += p.radius; m_vertices[quad+3].position = pos;
		pos.x -= p.radius; m_vertices[quad+0].position = pos;
	}
}

//------------------------------------------------------------------------------
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	if(r_texture == nullptr)
		log.err() << "ParticleSystem::draw: no atlas defined !" << log.endl();
#endif
	// apply texture
	states.texture = r_texture;

	states.transform = entity().transform.matrix();

	// draw the vertex array
	target.draw(m_vertices, states);

	// Test debug
//	sf::RectangleShape s(sf::Vector2f(32,32));
//	for(u32 i = 0; i < m_particles.size(); ++i)
//	{
//		s.setPosition(m_particles[i].pos);
//		target.draw(s);
//	}
}

//------------------------------------------------------------------------------
void ParticleSystem::serializeData(JsonBox::Value & o)
{
	Renderer::serializeData(o);

	o["maxParticles"] = (s32)m_maxParticles;

	zn::serialize(o["particleRadius"], m_particleRadius);
	zn::serialize(o["particleLifeTime"], m_particleLifeTime);
	zn::serialize(o["emissionZone"], m_emissionZone);

	std::string textureName;
	if(r_texture != nullptr)
	{
		textureName = AssetBank::current()->textures.findName(r_texture);
	}
	o["texture"] = textureName;

	std::string atlasName;
	if(r_atlas != nullptr)
	{
		atlasName = AssetBank::current()->atlases.findName(r_atlas);
	}
	o["atlas"] = atlasName;
	zn::serialize(o["atlasRect"], m_atlasRect);
}

//------------------------------------------------------------------------------
void ParticleSystem::unserializeData(JsonBox::Value & o)
{
	Renderer::unserializeData(o);

	setMaxParticles(o["maxParticles"].getInt());

	zn::unserialize(o["particleRadius"], m_particleRadius);
	zn::unserialize(o["particleLifeTime"], m_particleLifeTime);
	zn::unserialize(o["emissionZone"], m_emissionZone);

	std::string textureName = o["texture"].getString();
	r_texture = AssetBank::current()->textures.get(textureName);
	if(r_texture == nullptr)
	{
		log.err() << "ParticleSystem::unserializeData: texture not found \""
			<< textureName << '"' << log.endl();
	}

	std::string atlasName = o["atlas"].getString();
	r_atlas = AssetBank::current()->atlases.get(atlasName);
	if(r_atlas == nullptr)
	{
		log.err() << "ParticleSystem::unserializeData: atlas not found \""
			<< atlasName << '"' << log.endl();
	}

	sf::IntRect atlasRect;
	zn::unserialize(o["atlasRect"], atlasRect);
	setTextureRect(atlasRect);
}

//------------------------------------------------------------------------------
void ParticleSystem::postUnserialize()
{

}

} // namespace zn



