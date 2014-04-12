#ifndef HEADER_ZN_SPRITEBATCH_HPP_INCLUDED
#define HEADER_ZN_SPRITEBATCH_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <fm/types.hpp>

namespace zn
{

/// \brief Quick helper to display sprites on-the-fly in an efficient way.
/// Drawing a huge bunch of sprites may be faster with this than using sf::Sprites.
class SpriteBatch
{
public:

	// TODO add more primitives
	// TODO use this as the main rendering layer over SFML?

	/// \brief How many vertices contains the batch on creation.
	static const u32 DEFAULT_CAPACITY = 4*100;
	/// \brief Maximum number of vertices the batch can contain.
	/// Over that limit, the batch executes a draw call and resets its vertex buffer.
	static const u32 DEFAULT_MAX_CAPACITY = 65536;

	SpriteBatch();
	~SpriteBatch();

	/// \brief Sets the target where the batch will render things
	void setRenderTarget(sf::RenderTarget & rt);

	/// \brief Adds a sprite to the batch (variant).
	/// \param center: the sprite will be centered here
	/// \param scale: scale factor to use
	/// \param color: color tint
	void drawSprite(
		sf::Texture * texture,
		sf::Vector2f center,
		sf::Vector2f scale = sf::Vector2f(1,1),
		sf::Color color = sf::Color::White
	);

	/// \brief Adds a sprite to the batch (variant).
	/// the whole texture will be mapped.
	void drawSprite(
		sf::Texture * texture,
		sf::FloatRect dstRect,
		sf::Color color = sf::Color::White
	);

	/// \brief Adds a sprite to the batch.
	/// \param texture: texture of the sprite
	/// \param srcRect: sub-rectangle of the texture used by the sprite
	/// \param dstRect: destination rectangle of the sprite to draw
	/// \param color: color tint of the sprite
	void drawSprite(
		sf::Texture * texture,
		sf::FloatRect srcRect,
		sf::FloatRect dstRect,
		sf::Color color = sf::Color::White
	);

	/// \brief really draws all sprites/vertices gathered in the batch on the render texture,
	/// in one draw call. Also clears the batch.
	void display();

	/// \brief Indicator: how many draw calls made so far. You can reset it whenever you want.
	u32 drawCallsCount;

	/// \brief Indicator: how many batch calls made so far.
	/// You can reset it whenever you want.
	/// \note you can calculate how many draw calls have been saved by batching
	/// by subtracting drawCallsCount.
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

