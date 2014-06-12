#ifndef HEADER_ZN_GUIEVENT_HPP_INCLUDED
#define HEADER_ZN_GUIEVENT_HPP_INCLUDED

namespace zn
{

struct GUIEvent
{
	sf::EventType type;

	struct Mouse
	{
		sf::Vector2f localPosition;
		sf::Vector2i screenPosition;
		sf::Vector2f lastLocalPosition;
		sf::Vector2i lastScreenPosition;
		sf::Mouse::Button button;
		s32 wheelDelta;
	};

	struct Keyboard
	{
		s32 unicode;
		sf::Keyboard::Key code;
	};

//	struct Joystick
//	{
//		// TODO
//	};

	Keyboard key;
	Mouse mouse;

	sf::Vector2i screenSize;

};

} // namespace zn

#endif // HEADER_ZN_GUIEVENT_HPP_INCLUDED



