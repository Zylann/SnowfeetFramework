#ifndef HEADER_DEMO_CURSOR_HPP_INCLUDED
#define HEADER_DEMO_CURSOR_HPP_INCLUDED

#include <fm/proto/behaviour.hpp>

namespace demo
{

class Cursor : public zn::ABehaviour
{
public:

	ZN_BEHAVIOUR(demo::Cursor, 0) // The cursor is updated first

//	void onCreate() override;
	void onStart() override;
	void onUpdate() override;

};

} // namespace demo

#endif // HEADER_DEMO_CURSOR_HPP_INCLUDED
