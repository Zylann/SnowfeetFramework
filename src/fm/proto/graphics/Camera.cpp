#include "../Entity.hpp"
#include "../Scene.hpp"
#include "Camera.hpp"

namespace zn
{

Camera::Camera() : AComponent(),
	m_scaleMode(FIXED)
{
}

Camera::~Camera()
{
}

void Camera::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().cameras.registerComponent(this);
}

void Camera::onDestroy()
{
	entity().scene().cameras.unregisterComponent(this);
}

void Camera::init()
{
}

void Camera::setSize(const sf::Vector2f & s)
{
	m_view.setSize(s);
}

void Camera::setViewport(const sf::FloatRect & r)
{
	m_view.setViewport(r);
}

void Camera::update()
{
	m_view.setCenter(entity().position());
	m_view.setRotation(entity().rotation());
}

void Camera::setScaleMode(u8 mode)
{
	m_scaleMode = mode;
}

void Camera::onScreenResized(sf::Vector2u resolution)
{
	if(m_scaleMode == FIXED)
	{
		m_view.setSize(resolution.x, resolution.y);
	}
	else if(m_scaleMode == ADAPTED)
	{
		sf::Vector2f s = m_view.getSize();
		f32 ratio = static_cast<f32>(resolution.y) / static_cast<f32>(resolution.x);
		m_view.setSize(s.x, ratio*s.x);
	}
}

//------------------------------------------------------------------------------
void Camera::serializeData(JsonBox::Value & o)
{
	AComponent::serializeData(o);

	JsonBox::Value & sizeData = o["size"];
	sizeData["x"] = m_view.getSize().x;
	sizeData["y"] = m_view.getSize().y;

	JsonBox::Value & viewportData = o["viewport"];
	viewportData["x"] = m_view.getViewport().left;
	viewportData["y"] = m_view.getViewport().top;
	viewportData["w"] = m_view.getViewport().width;
	viewportData["h"] = m_view.getViewport().height;

	std::string scaleModeStr;
	switch(m_scaleMode)
	{
	case FIXED: scaleModeStr = "fixed"; break;
	case ADAPTED: scaleModeStr = "adapted"; break;
	case STRETCHED: scaleModeStr = "stretched"; break;
	default: break;
	}
	o["scaleMode"] = scaleModeStr;
}

//------------------------------------------------------------------------------
void Camera::unserializeData(JsonBox::Value & o)
{
	AComponent::unserializeData(o);

	JsonBox::Value & sizeData = o["size"];
	m_view.setSize(
		sizeData["x"].getDouble(),
		sizeData["y"].getDouble()
	);

	JsonBox::Value & viewportData = o["viewport"];
	m_view.setViewport(sf::FloatRect(
		viewportData["x"].getDouble(),
		viewportData["y"].getDouble(),
		viewportData["w"].getDouble(),
		viewportData["h"].getDouble()
	));

	std::string scaleModeStr = o["scaleMode"].getString();
	if(scaleModeStr == "fixed")
	{
		m_scaleMode = FIXED;
	}
	else if(scaleModeStr == "adapted")
	{
		m_scaleMode = ADAPTED;
	}
	else if(scaleModeStr == "stretched")
	{
		m_scaleMode = STRETCHED;
	}
}

//------------------------------------------------------------------------------
void Camera::postUnserialize()
{

}

} // namespace zn




