#include <fm/scene/gui/GUIRoot.hpp>
#include <fm/json/json_utils.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void GUIRoot::setCamera(Camera* guiCamera)
{
	r_guiCamera = guiCamera;
}

//------------------------------------------------------------------------------
bool GUIRoot::onEvent(const sf::Event& event)
{
	return false;
}

//------------------------------------------------------------------------------
void GUIRoot::serializeData(JsonBox::Value& o)
{
	AComponent::serializeData(o);
	zn::serialize(o["rect"], m_rect);
}

//------------------------------------------------------------------------------
void GUIRoot::unserializeData(JsonBox::Value& o)
{
	AComponent::unserializeData(o);
	zn::unserialize(o["rect"], m_rect);
}

//------------------------------------------------------------------------------
void GUIRoot::postUnserialize()
{
	// TODO relink camera
}


} // namespace zn

