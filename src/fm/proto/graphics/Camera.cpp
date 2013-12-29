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

} // namespace zn




