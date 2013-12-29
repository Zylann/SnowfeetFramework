#ifndef HEADER_ZN_PARTICLESYSTEM_HPP_INCLUDED
#define HEADER_ZN_PARTICLESYSTEM_HPP_INCLUDED

#include "../../util/Range.hpp"
#include "Renderer.hpp"

namespace zn
{

class ParticleSystem : public ARenderer
{
public:

	ZN_COMPONENT(zn::ParticleSystem, CG_RENDERER)

	ParticleSystem() : ARenderer(),
		_maxParticles(0),
		_particleRadius(1,1),
		_particleLifeTime(10,10),
		_atlasRef(nullptr)
	{
		_vertices.setPrimitiveType(sf::Quads);
	}

	const TextureAtlas * atlas() override { return _atlasRef; }
	void setAtlas(const TextureAtlas * atlas) override;
	void setTextureRect(const sf::IntRect & rect) override;

	void setMaxParticles(u32 max);
	void setParticleRadius(f32 min, f32 max);
	void setParticleLifeTime(f32 min, f32 max);
	void setEmissionZone(const sf::FloatRect & rect);

protected:

	void init() override;
	void update() override;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	void updateUVs(u32 i_start);

	u32 _maxParticles;
	Range<f32> _particleRadius;
	Range<f32> _particleLifeTime;
	sf::FloatRect _emissionZone;
	const TextureAtlas * _atlasRef;
	sf::IntRect _atlasRect;
	sf::VertexArray _vertices;

	struct Particle
	{
		sf::Vector2f pos;
		sf::Vector2f velocity;
		f32 remainingTime;
		f32 radius;

		Particle() : remainingTime(-1), radius(1) {}
	};

	std::vector<Particle> _particles;

};

} // namespace zn

#endif // HEADER_ZN_PARTICLESYSTEM_HPP_INCLUDED



