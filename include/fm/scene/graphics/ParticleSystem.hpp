#ifndef HEADER_ZN_PARTICLESYSTEM_HPP_INCLUDED
#define HEADER_ZN_PARTICLESYSTEM_HPP_INCLUDED

#include <fm/util/Range.hpp>
#include <fm/scene/graphics/Renderer.hpp>

namespace zn
{

class ZN_API ParticleSystem : public Renderer
{
public:

	ZN_COMPONENT(zn::ParticleSystem, zn::Renderer, 0)

	ParticleSystem() : Renderer(),
		m_maxParticles(0),
		m_particleRadius(1,1),
		m_particleLifeTime(10,10),
		r_atlas(nullptr),
		r_texture(nullptr)
	{
		m_vertices.setPrimitiveType(sf::Quads);
	}

	const TextureAtlas * atlas() const override { return r_atlas; }
	void setAtlas(const TextureAtlas * atlas) override;

	const sf::Texture * texture() const override { return r_texture; }
	void setTexture(const sf::Texture * texture) override;

	void setTextureRect(const sf::IntRect & rect) override;

	sf::FloatRect localBounds() const override;

	void setMaxParticles(u32 max);
	void setParticleRadius(f32 min, f32 max);
	void setParticleLifeTime(f32 min, f32 max);
	void setEmissionZone(const sf::FloatRect & rect);

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

protected:

	void onCreate() override;
	void onUpdate() override;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	void updateUVs(u32 i_start);

	u32                    m_maxParticles;
	Range<f32>             m_particleRadius;
	Range<f32>             m_particleLifeTime;
	sf::FloatRect          m_emissionZone; // TODO more particle shapes
	const TextureAtlas *   r_atlas;
	const sf::Texture *    r_texture;
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



