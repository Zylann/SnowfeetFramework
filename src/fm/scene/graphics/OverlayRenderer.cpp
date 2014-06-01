#include <fm/scene/graphics/OverlayRenderer.hpp>
#include <fm/asset/AssetBank.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

//------------------------------------------------------------------------------
OverlayRenderer::OverlayRenderer() : Renderer(),
	r_texture(nullptr),
	m_color(255,255,255)
{
}

//------------------------------------------------------------------------------
void OverlayRenderer::setTexture(const sf::Texture* texture)
{
	r_texture = texture;
}

//------------------------------------------------------------------------------
void OverlayRenderer::setColor(sf::Color color)
{
	m_color = color;
}

//------------------------------------------------------------------------------
sf::FloatRect OverlayRenderer::localBounds() const
{
	return sf::FloatRect(-99999, -99999, 199999, 199999);
}

//------------------------------------------------------------------------------
void OverlayRenderer::serializeData(JsonBox::Value& o)
{
	Renderer::serializeData(o);

	zn::serialize(o["color"], m_color);

	if(r_texture != nullptr)
	{
		std::string textureName = AssetBank::current()->textures.findName(r_texture);
		o["texture"] = textureName;
	}
	else
	{
		o["texture"].setNull();
	}
}

//------------------------------------------------------------------------------
void OverlayRenderer::unserializeData(JsonBox::Value& o)
{
	Renderer::unserialize(o);

	zn::unserialize(o["color"], m_color);

	std::string textureName = o["texture"].getString();
	if(textureName.empty())
	{
		log.err() << "OverlayRenderer::unserialize: texture name is empty" << log.endl();
	}
	else
	{
		const sf::Texture * texture = AssetBank::current()->textures.get(textureName);
		if(texture != nullptr)
		{
			setTexture(texture);
		}
		else
		{
			log.err() << "OverlayRenderer::unserializeData: "
				"texture not found \"" << textureName << '"' << log.endl();
		}
	}
}

//------------------------------------------------------------------------------
void OverlayRenderer::postUnserialize()
{
}

//------------------------------------------------------------------------------
void OverlayRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = sf::Transform::Identity;
	states.texture = r_texture;
	//states.transform.combine(entity().transform.matrix());

	const sf::View & view = entity().scene().renderSystem.activeCamera()->internalView();
	sf::FloatRect viewBounds(view.getCenter()-view.getSize()*0.5f, view.getSize());

	sf::Vector2u ts = r_texture->getSize();

	sf::VertexArray mesh;
	mesh.setPrimitiveType(sf::PrimitiveType::Quads);
	mesh.resize(4);

	mesh[0] = sf::Vertex(sf::Vector2f(viewBounds.left, viewBounds.top), m_color, sf::Vector2f(0, 0));
	mesh[1] = sf::Vertex(sf::Vector2f(viewBounds.left+viewBounds.width, viewBounds.top), m_color, sf::Vector2f(ts.x, 0));
	mesh[2] = sf::Vertex(sf::Vector2f(viewBounds.left+viewBounds.width, viewBounds.top+viewBounds.height), m_color, sf::Vector2f(ts.x, ts.y));
	mesh[3] = sf::Vertex(sf::Vector2f(viewBounds.left, viewBounds.top+viewBounds.height), m_color, sf::Vector2f(0, ts.y));

	target.draw(mesh, states);
	//target.draw(sf::RectangleShape(sf::Vector2f(10,10)), states);
}

} // namespace zn


