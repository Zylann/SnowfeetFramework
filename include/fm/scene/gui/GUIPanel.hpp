#ifndef HEADER_ZN_GUIPANEL_HPP_INCLUDED
#define HEADER_ZN_GUIPANEL_HPP_INCLUDED

#include <fm/scene/graphics/Camera.hpp>

namespace zn
{

class GUIPanel : public Behaviour
{
public:

	ZN_OBJECT(zn::GUIPanel, zn::Behaviour, 0)

	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;

private:

	sf::FloatRect m_rect;

};

} // namespace zn

#endif // HEADER_ZN_GUIPANEL_HPP_INCLUDED

