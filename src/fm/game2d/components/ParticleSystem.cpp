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
	_atlasRef = atlas;
}

//------------------------------------------------------------------------------
void ParticleSystem::setTextureRect(const sf::IntRect& rect)
{
	if(_atlasRef == nullptr)
		return;
#ifdef ZN_DEBUG
	if(!checkTextureRect(_atlasRef->texture(), rect))
		std::cout << "E: ParticleSystem::setTextureRect: invalid rectangle ("
			<< rect.left << ", " << rect.top << "; "
			<< rect.width << ", " << rect.height << ")" << std::endl;
#endif
	_atlasRect = rect;
}

//------------------------------------------------------------------------------
void ParticleSystem::setMaxParticles(u32 max)
{
	if(_maxParticles != max)
	{
		if(max == 0)
		{
			_particles.clear();
			_vertices.clear();
		}
		else
		{
			// TODO wait until enough particles fade

			_particles.resize(max);
			_vertices.resize(4*max);

			if(max > _maxParticles)
			{
				updateUVs(_maxParticles);
			}
		}

		_maxParticles = max;
	}
}

//------------------------------------------------------------------------------
void ParticleSystem::updateUVs(u32 i_start)
{
	if(_atlasRef == nullptr)
	{
		std::cout << "E: ParticleSystem::updateUVs: no atlas defined !" << std::endl;
		return;
	}

	f32 minX = _atlasRect.left;
	f32 minY = _atlasRect.top;
	f32 maxX = _atlasRect.left+_atlasRect.width-1;
	f32 maxY = _atlasRect.top+_atlasRect.height-1;

	for(u32 i = i_start; i < _particles.size(); ++i)
	{
		// Set UVs counter-clockwise
		sf::Vertex * quad = &_vertices[i*4];
		quad[0].texCoords = sf::Vector2f(minX, minY);
		quad[1].texCoords = sf::Vector2f(maxX, minY);
		quad[2].texCoords = sf::Vector2f(maxX, maxY);
		quad[3].texCoords = sf::Vector2f(minX, maxY);
	}
}

//------------------------------------------------------------------------------
void ParticleSystem::setParticleRadius(f32 min, f32 max)
{
	_particleRadius.set(min, max);
}

//------------------------------------------------------------------------------
void ParticleSystem::setParticleLifeTime(f32 min, f32 max)
{
	_particleLifeTime.set(min, max);
}

//------------------------------------------------------------------------------
// TODO more emission zone types
void ParticleSystem::setEmissionZone(const sf::FloatRect& rect)
{
	_emissionZone = rect;
}

//------------------------------------------------------------------------------
void ParticleSystem::init()
{
}

//------------------------------------------------------------------------------
void ParticleSystem::update()
{
	//sf::Vector2f ownerPos = owner().position();

	for(u32 i = 0; i < _particles.size(); ++i)
	{
		Particle & p = _particles[i];

		if(p.remainingTime < 0)
		{
			p.remainingTime = math::randf(_particleLifeTime.min(), _particleLifeTime.max());

			p.pos.x = math::randf(_emissionZone.left, _emissionZone.left+_emissionZone.width);
			p.pos.y = math::randf(_emissionZone.top, _emissionZone.top+_emissionZone.height);

			// TODO customize velocity
			p.velocity.x = math::randf(-0.5f, 0.5f);
			p.velocity.y = math::randf(-0.5f, 0.5f);

			p.radius = math::randf(_particleRadius.min(), _particleRadius.max());
		}
		else
		{
			// TODO
			p.remainingTime -= owner().scene().deltaTime().asSeconds();
			p.pos += p.velocity;
		}

		// Update mesh quads

		u32 quad = i*4;
		sf::Vector2f pos(p.pos.x - 0.5f*p.radius, p.pos.y - 0.5f*p.radius);
		// Counter-clockwise quad
		                   _vertices[quad+1].position = pos;
		pos.x += p.radius; _vertices[quad+2].position = pos;
		pos.y += p.radius; _vertices[quad+3].position = pos;
		pos.x -= p.radius; _vertices[quad+0].position = pos;
	}
}

//------------------------------------------------------------------------------
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	if(_atlasRef == nullptr)
		std::cout << "E: ParticleSystem::draw: no atlas defined !" << std::endl;
#endif
	// apply texture
	states.texture = &_atlasRef->texture();

	// draw the vertex array
	target.draw(_vertices, states);

	// Test debug
//	sf::RectangleShape s(sf::Vector2f(32,32));
//	for(u32 i = 0; i < _particles.size(); ++i)
//	{
//		s.setPosition(_particles[i].pos);
//		target.draw(s);
//	}
}


} // namespace zn



