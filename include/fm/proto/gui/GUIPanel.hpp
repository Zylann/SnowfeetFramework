#ifndef HEADER_ZN_GUIPANEL_HPP_INCLUDED
#define HEADER_ZN_GUIPANEL_HPP_INCLUDED

#include <fm/proto/graphics/Camera.hpp>

namespace zn
{

class GUIPanel : public ABehaviour
{
public:

	ZN_BEHAVIOUR(zn::GUIPanel, 0)

	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;

private:

	sf::FloatRect m_rect;

};

} // namespace zn

#endif // HEADER_ZN_GUIPANEL_HPP_INCLUDED

