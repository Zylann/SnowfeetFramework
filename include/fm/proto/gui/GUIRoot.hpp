#ifndef GUIROOT_HPP_INCLUDED
#define GUIROOT_HPP_INCLUDED

#include <fm/proto/behaviour/Behaviour.hpp>
#include <fm/proto/graphics/Camera.hpp>

namespace zn
{

class GUIRoot : public ABehaviour
{
public:

	ZN_BEHAVIOUR(zn::GUIRoot, 0)

	GUIRoot() :
		r_guiCamera(nullptr)
	{}

	void setCamera(Camera * guiCamera);

	bool onEvent(const sf::Event & event) override;

	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

private:

	Camera * r_guiCamera;
	sf::FloatRect m_rect;

};

} // namespace zn

#endif // GUIROOT_HPP_INCLUDED
