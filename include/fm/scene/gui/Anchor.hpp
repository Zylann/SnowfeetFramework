#ifndef HEADER_ZN_ANCHOR_HPP_INCLUDED
#define HEADER_ZN_ANCHOR_HPP_INCLUDED

#include <fm/scene/behaviour/Behaviour.hpp>

namespace zn
{

class ZN_API Anchor : public ABehaviour
{
public:

	ZN_COMPONENT(zn::Anchor, zn::Behaviour, 0)

	bool onEvent(const sf::Event & event) override;

	void setAlign(sf::Vector2f align);
	inline const sf::Vector2f & align() const { return m_align; }

	void serializeData(JsonBox::Value & o);
	void unserializeData(JsonBox::Value & o);

private:

	void apply();

	sf::Vector2f m_align;

};

} // namespace zn

#endif // HEADER_ZN_ANCHOR_HPP_INCLUDED
