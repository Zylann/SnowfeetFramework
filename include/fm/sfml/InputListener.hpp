#ifndef HEADER_ZN_EVENTLISTENER_HPP_INCLUDED
#define HEADER_ZN_EVENTLISTENER_HPP_INCLUDED

#include <SFML/Window.hpp>
#include <fm/types.hpp>

namespace zn
{

class ZN_API IInputListener
{
public:

	virtual ~IInputListener() {}

	virtual bool onEvent(const sf::Event & event) { return false; }

protected:

	IInputListener() {}

};

} // namespace zn

#endif // HEADER_ZN_EVENTLISTENER_HPP_INCLUDED
