#include "ParticleSystem.hpp"
#include "../../sfml/sfml2_utils.hpp"
#include "../Entity.hpp"
#include "../../util/math.hpp"
#include "../Scene.hpp"

namespace zn
{

//------------------------------------------------------------------------------
void ParticleSystem::setAtlas(const TextureAtlas* atlas)
{
	r_atlas = atlas;
}

//------------------------------------------------------------------------------
void ParticleSystem::setTextureRect(const sf::IntRect& rect)
{
	if(r_atlas == nullptr)
		return;
#ifdef ZN_DEBUG
	if(!checkTextureRect(r_atlas->texture(), rect))
		std::cout << "E: ParticleSystem::setTextureRect: invalid rectangle ("
			<< rect.left << ", " << rect.top << "; "
			<< rect.width << ", " << rect.height << ")" << std::endl;
#endif
	m_atlasRect = rect;
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
	if(r_atlas == nullptr)
	{
		std::cout << "E: ParticleSystem::updateUVs: no atlas defined !" << std::endl;
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
void ParticleSystem::init()
{
}

//------------------------------------------------------------------------------
void ParticleSystem::update()
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
	if(r_atlas == nullptr)
		std::cout << "E: ParticleSystem::draw: no atlas defined !" << std::endl;
#endif
	// apply texture
	states.texture = &r_atlas->texture();

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


} // namespace zn



