#ifndef HEADER_ZN_SPRITEBATCH_HPP_INCLUDED
#define HEADER_ZN_SPRITEBATCH_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <fm/types.hpp>

namespace zn
{

/// \brief Quick helper to display sprites on-the-fly in an efficient way
class SpriteBatch
{
public:

	static const u32 DEFAULT_CAPACITY = 4*100;
	static const u32 DEFAULT_MAX_CAPACITY = 65536;

	SpriteBatch();
	~SpriteBatch();

	void setRenderTarget(sf::RenderTarget & rt);

	void drawSprite(
		sf::Texture * texture,
		sf::Vector2f center,
		sf::Vector2f scale = sf::Vector2f(1,1),
		sf::Color color = sf::Color::White
	);

	void drawSprite(
		sf::Texture * texture,
		sf::FloatRect dstRect,
		sf::Color color = sf::Color::White
	);

	void drawSprite(
		sf::Texture * texture,
		sf::FloatRect srcRect,
		sf::FloatRect dstRect,
		sf::Color color = sf::Color::White
	);

	void display();

	u32 drawCallsCount;
	u32 batchCallsCount;

private:

	sf::Vertex * m_vertices;
	u32 m_capacity;
	u32 m_maxCapacity;

	u32 m_primitiveCount;
	u32 m_primitiveSize;
	sf::PrimitiveType m_primitive;

	sf::RenderTarget * r_renderTarget;
	sf::RenderStates m_renderStates;

};

} // namespace zn

#endif // HEADER_ZN_SPRITEBATCH_HPP_INCLUDED

