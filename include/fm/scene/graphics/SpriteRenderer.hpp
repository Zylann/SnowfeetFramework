#ifndef HEADER_ZN_SPRITERENDERER_HPP_INCLUDED
#define HEADER_ZN_SPRITERENDERER_HPP_INCLUDED

#include <fm/scene/graphics/Renderer.hpp>

namespace zn
{

class ZN_API SpriteRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::SpriteRenderer, zn::ARenderer, 0)

	SpriteRenderer();

	void setColor(sf::Color color);
	const sf::Color & color() const;

	inline void setScale(f32 scaleXY) { setScale(sf::Vector2f(scaleXY, scaleXY)); }
	void setScale(const sf::Vector2f & scale);
	inline const sf::Vector2f & scale() const { return m_scale; }
	void scaleToPixels();

	sf::FloatRect localBounds() const override;

	void setTexture(const sf::Texture * texture) override;
	void setTextureRect(const sf::IntRect & rect) override;
	const sf::Texture * texture() const override { return r_texture; }

	const TextureAtlas * atlas() const override { return r_atlas; }
	void setAtlas(const TextureAtlas * atlas) override;
	void setFrame(const std::string& id);

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;

protected:

	void updateVertices();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	sf::VertexArray m_vertices;
	sf::IntRect m_textureRect;
	const sf::Texture * r_texture;
	const TextureAtlas * r_atlas;
	sf::Vector2f m_scale;

};

} // namespace zn

#endif // HEADER_ZN_SPRITERENDERER_HPP_INCLUDED


