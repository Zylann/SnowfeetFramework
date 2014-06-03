#ifndef HEADER_DEMO_CURSOR_HPP_INCLUDED
#define HEADER_DEMO_CURSOR_HPP_INCLUDED

#include <fm/scene/behaviour.hpp>

namespace demo
{

class Cursor : public zn::Behaviour
{
public:

	ZN_OBJECT(demo::Cursor, zn::Behaviour)

	void onCreate() override;
//	void onStart() override;
	void onUpdate() override;

};

} // namespace demo

#endif // HEADER_DEMO_CURSOR_HPP_INCLUDED
