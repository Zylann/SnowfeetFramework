#ifndef SSM_CURSOR_HPP_INCLUDED
#define SSM_CURSOR_HPP_INCLUDED

#include "fm/proto/behaviour/Behaviour.hpp"

namespace demo
{

class Cursor : public zn::ABehaviour
{
public:

	ZN_BEHAVIOUR(demo::Cursor, 0) // The cursor is updated first

	void init() override;
	void onStart() override;
	void onUpdate() override;

};

} // namespace demo

#endif // SSM_CURSOR_HPP_INCLUDED
