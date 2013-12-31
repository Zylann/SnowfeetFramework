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
		m_maxParticles(0),
		m_particleRadius(1,1),
		m_particleLifeTime(10,10),
		r_atlas(nullptr)
	{
		m_vertices.setPrimitiveType(sf::Quads);
	}

	const TextureAtlas * atlas() override { return r_atlas; }
	void setAtlas(const TextureAtlas * atlas) override;
	void setTextureRect(const sf::IntRect & rect) override;

	void setMaxParticles(u32 max);
	void setParticleRadius(f32 min, f32 max);
	void setParticleLifeTime(f32 min, f32 max);
	void setEmissionZone(const sf::FloatRect & rect);

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

protected:

	void init() override;
	void update() override;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	void updateUVs(u32 i_start);

	u32                    m_maxParticles;
	Range<f32>             m_particleRadius;
	Range<f32>             m_particleLifeTime;
	sf::FloatRect          m_emissionZone;
	const TextureAtlas *   r_atlas;
	sf::IntRect            m_atlasRect;
	sf::VertexArray        m_vertices;

	struct Particle
	{
		sf::Vector2f pos;
		sf::Vector2f velocity;
		f32 remainingTime;
		f32 radius;

		Particle() : remainingTime(-1), radius(1) {}
	};

	std::vector<Particle>  m_particles;

};

} // namespace zn

#endif // HEADER_ZN_PARTICLESYSTEM_HPP_INCLUDED



