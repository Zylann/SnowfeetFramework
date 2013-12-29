#ifndef HEADER_ZN_SPRITERENDERER_H_INCLUDED
#define HEADER_ZN_SPRITERENDERER_H_INCLUDED

#include "Renderer.hpp"

namespace zn
{

class SpriteRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::SpriteRenderer, CG_RENDERER)

	SpriteRenderer() : ARenderer(),
		r_atlas(nullptr)
	{}

	inline void setPosition(const sf::Vector2f & pos) { m_sprite.setPosition(pos); }
	inline void setScale(const sf::Vector2f & scale) { m_sprite.setScale(scale); }
	inline void setOrigin(const sf::Vector2f & org) { m_sprite.setOrigin(org); }
	inline void setRotation(float angleDeg) { m_sprite.setRotation(angleDeg); }

	inline const sf::Vector2f & position() const { return m_sprite.getPosition(); }
	inline const sf::Vector2f & scale() const { return m_sprite.getScale(); }
	inline const sf::Vector2f & origin() const { return m_sprite.getOrigin(); }
	inline float rotation() const { return m_sprite.getRotation(); }

	void setTexture(const sf::Texture & texture) { m_sprite.setTexture(texture); }
	void setTextureRect(const sf::IntRect & rect) override;

	const TextureAtlas * atlas() override { return r_atlas; }
	void setAtlas(const TextureAtlas * atlas) override;

	void setFrame(const std::string& id);

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	sf::Sprite m_sprite;
	const TextureAtlas * r_atlas;

};

} // namespace zn

#endif // HEADER_ZN_SPRITERENDERER_H_INCLUDED


