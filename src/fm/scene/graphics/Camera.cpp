#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/Scene.hpp>
#include <fm/scene/graphics/Camera.hpp>
#include <fm/app/Application.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

ZN_OBJECT_IMPL(zn::Camera)

//------------------------------------------------------------------------------
Camera::Camera() : Component(),
	depth(0),
	layerMask(1), // See first layer by default
	doClear(true),
	clearColor(0,0,0),
	m_scaleMode(FIXED),
	m_fixedZoom(1),
	r_renderTexture(nullptr)
{
}

//------------------------------------------------------------------------------
Camera::~Camera()
{
}

//------------------------------------------------------------------------------
void Camera::onAdd(Entity * e)
{
	Component::onAdd(e);
	entity().scene().renderSystem.registerCamera(this);
}

//------------------------------------------------------------------------------
void Camera::onDestroy()
{
	entity().scene().renderSystem.unregisterCamera(this);
}

//------------------------------------------------------------------------------
void Camera::onCreate()
{
	// Note: Application should never be null here, because it's the first object
	// to be created when the engine runs
	sf::Vector2i screenSize = Application::instance()->screenSize();
	onScreenResized(sf::Vector2u(screenSize.x, screenSize.y));
}

//------------------------------------------------------------------------------
void Camera::setSize(const sf::Vector2f & s)
{
	m_view.setSize(s);
}

//------------------------------------------------------------------------------
void Camera::setFixedZoom(f32 fixedZoom)
{
	const f32 epsilon = 0.001f;
	if(fixedZoom < epsilon)
	{
		m_fixedZoom = epsilon;
#ifdef ZN_DEBUG
		std::cout << "W: Camera::setFixedZoom: too tiny (" << fixedZoom << ")" << std::endl;
#endif
	}
	else
	{
		m_fixedZoom = fixedZoom;
	}

	sf::Vector2i screenSize = Application::instance()->screenSize();
	onScreenResized(sf::Vector2u(screenSize.x, screenSize.y));
}

//------------------------------------------------------------------------------
void Camera::setViewport(const sf::FloatRect & r)
{
	m_view.setViewport(r);
}

//------------------------------------------------------------------------------
void Camera::setRenderTexture(sf::RenderTexture * targetTexture)
{
	r_renderTexture = targetTexture;
}

//------------------------------------------------------------------------------
sf::Vector2f Camera::screenToWorld(const sf::Vector2i & screenPoint) const
{
	sf::Vector2i screenSize = Application::instance()->screenSize();

	sf::Vector2f vpos(
		2.f * (static_cast<f32>(screenPoint.x) / static_cast<f32>(screenSize.x)) - 1.f,
		2.f * (1.f - static_cast<f32>(screenPoint.y) / static_cast<f32>(screenSize.y)) - 1.f
	);

	return m_view.getInverseTransform().transformPoint(vpos);
}

//------------------------------------------------------------------------------
sf::Vector2f Camera::worldToScreen(const sf::Vector2f & worldPoint) const
{
	sf::Vector2i screenSize = Application::instance()->screenSize();

	sf::Vector2f vpos = m_view.getTransform().transformPoint(worldPoint);

	return sf::Vector2f(
		(0.5f * vpos.x + 0.5f) * static_cast<f32>(screenSize.x),
		(1.f - (0.5f * vpos.y + 0.5f)) * static_cast<f32>(screenSize.y)
	);
}

//------------------------------------------------------------------------------
void Camera::onUpdate()
{
	if(entity().transform())
	{
		m_view.setCenter(entity().transform()->position());
		m_view.setRotation(entity().transform()->rotation());
	}
}

//------------------------------------------------------------------------------
void Camera::setScaleMode(u8 mode)
{
	m_scaleMode = mode;
}

//------------------------------------------------------------------------------
void Camera::onScreenResized(sf::Vector2u resolution)
{
	if(m_scaleMode == FIXED)
	{
		// Take screen resolution and zoom it
		m_view.setSize(
			static_cast<f32>(resolution.x) / m_fixedZoom,
			static_cast<f32>(resolution.y) / m_fixedZoom
		);
	}
	else if(m_scaleMode == ADAPTED)
	{
		// Take the current size and modify it according to screen resolution to keep the same height
		sf::Vector2f s = m_view.getSize();
		f32 ratio = static_cast<f32>(resolution.y) / static_cast<f32>(resolution.x);
		m_view.setSize(s.x, ratio*s.x);
	}
	//else if(m_scaleMode == STRETCHED)
	//{
		// Do nothing, just leave the defined view size
	//}
}

//------------------------------------------------------------------------------
void Camera::serializeData(JsonBox::Value & o)
{
	Component::serializeData(o);

	o["depth"] = depth;

	zn::serialize(o["size"], m_view.getSize());
	zn::serialize(o["viewport"], m_view.getViewport());

	o["doClear"] = doClear;
	zn::serialize(o["clearColor"], clearColor);

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
	Component::unserializeData(o);

	depth = o["depth"].getInt();

	sf::Vector2f size;
	zn::unserialize(o["size"], size);
	m_view.setSize(size);

	sf::FloatRect viewport;
	zn::unserialize(o["viewport"], viewport);
	m_view.setViewport(viewport);

	doClear = o["doClear"].getBoolean();
	zn::unserialize(o["clearColor"], clearColor);

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

	// TODO add RenderTextures to AssetBank
	r_renderTexture = nullptr;
}

//------------------------------------------------------------------------------
void Camera::postUnserialize()
{

}

} // namespace zn




