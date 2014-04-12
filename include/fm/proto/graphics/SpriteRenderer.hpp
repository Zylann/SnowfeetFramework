#ifndef HEADER_ZN_SPRITERENDERER_HPP_INCLUDED
#define HEADER_ZN_SPRITERENDERER_HPP_INCLUDED

#include <fm/proto/graphics/Renderer.hpp>

namespace zn
{

class ZN_API SpriteRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::SpriteRenderer, CG_RENDERER, 0)

	SpriteRenderer();

	void setColor(sf::Color color);
	const sf::Color & color() const;

	void setScale(const sf::Vector2f & scale);
	void scaleToPixels(bool enable);
	inline bool scaledToPixels() const { return m_scaleToPixels; }

	sf::FloatRect localBounds() const override;

	void setTexture(const sf::Texture * texture) override;
	void setTexture(const sf::Texture * texture, bool resetRect);
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
	bool m_scaleToPixels;

};

} // namespace zn

#endif // HEADER_ZN_SPRITERENDERER_HPP_INCLUDED


