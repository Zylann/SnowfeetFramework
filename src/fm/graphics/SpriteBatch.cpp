#include <fm/graphics/SpriteBatch.hpp>
#include <iostream>
namespace zn
{

//------------------------------------------------------------------------------
SpriteBatch::SpriteBatch():
	drawCallsCount(0),
	batchCallsCount(0),
	m_vertices(nullptr),
	m_capacity(DEFAULT_CAPACITY),
	m_maxCapacity(DEFAULT_MAX_CAPACITY),
	m_primitiveCount(0),
	m_primitiveSize(4),
	m_primitive(sf::PrimitiveType::Quads),
	r_renderTarget(nullptr)
{
	m_vertices = new sf::Vertex[m_capacity];
}

//------------------------------------------------------------------------------
SpriteBatch::~SpriteBatch()
{
	delete m_vertices;
}

//------------------------------------------------------------------------------
void SpriteBatch::setRenderTarget(sf::RenderTarget& rt)
{
	if(&rt != r_renderTarget)
	{
		if(m_primitiveCount != 0 && r_renderTarget != nullptr)
		{
			display();
		}

		r_renderTarget = &rt;
	}
}

//------------------------------------------------------------------------------
void SpriteBatch::drawSprite(
		sf::Texture * texture,
		sf::Vector2f center,
		sf::Vector2f scale,
		sf::Color color)
{
	sf::Vector2u ts = texture->getSize();
	drawSprite(texture,
		sf::FloatRect(0, 0, ts.x, ts.y), // src
		sf::FloatRect(center.x - scale.x*ts.x/2,
					  center.y - scale.y*ts.y/2,
					  scale.x*ts.x,
					  scale.y*ts.y), // dst
		color
	);
}

//------------------------------------------------------------------------------
void SpriteBatch::drawSprite(
		sf::Texture * texture,
		sf::FloatRect dstRect,
		sf::Color color)
{
	drawSprite(texture,
		sf::FloatRect(0, 0,
					texture->getSize().x,
					texture->getSize().y), // src
		dstRect, // dst
		color
	);
}

//------------------------------------------------------------------------------
void SpriteBatch::drawSprite(
		sf::Texture* texture,
		sf::FloatRect srcRect,
		sf::FloatRect dstRect,
		sf::Color color)
{
	if(m_renderStates.texture != texture)
	{
		if(m_primitiveCount != 0)
			display();
		m_renderStates.texture = texture;
	}

	if(m_primitiveCount*m_primitiveSize >= m_capacity)
	{
		display();

		if(m_capacity < m_maxCapacity)
		{
			m_capacity *= 2;

			if(m_capacity > m_maxCapacity)
				m_capacity = m_maxCapacity;

			delete[] m_vertices;
			m_vertices = new sf::Vertex[m_capacity];
		}
	}

	sf::Vertex * quad = &(m_vertices[m_primitiveCount*m_primitiveSize]);

	quad[0] = sf::Vertex(
		sf::Vector2f(dstRect.left, dstRect.top), color,
		sf::Vector2f(srcRect.left, srcRect.top)
	);

	quad[1] = sf::Vertex(
		sf::Vector2f(dstRect.left+dstRect.width, dstRect.top), color,
		sf::Vector2f(srcRect.left+srcRect.width, srcRect.top)
	);

	quad[2] = sf::Vertex(
		sf::Vector2f(dstRect.left+dstRect.width, dstRect.top+dstRect.height), color,
		sf::Vector2f(srcRect.left+srcRect.width, srcRect.top+srcRect.height)
	);

	quad[3] = sf::Vertex(
		sf::Vector2f(dstRect.left, dstRect.top+dstRect.height), color,
		sf::Vector2f(srcRect.left, srcRect.top+srcRect.height)
	);

	++m_primitiveCount;
	++batchCallsCount;
}

//------------------------------------------------------------------------------
void SpriteBatch::display()
{
	//std::cout << m_primitiveCount << "," << m_primitiveSize << "," << m_capacity << " ";
	r_renderTarget->draw(m_vertices, m_primitiveCount*m_primitiveSize, m_primitive, m_renderStates);
	m_primitiveCount = 0;

	++drawCallsCount;
}

} // namespace zn


