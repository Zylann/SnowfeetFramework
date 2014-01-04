#ifndef SSM_CURSOR_HPP_INCLUDED
#define SSM_CURSOR_HPP_INCLUDED

#include "fm/proto/behaviour/Behaviour.hpp"

namespace ssm
{

class Cursor : public zn::ABehaviour
{
public:

	ZN_BEHAVIOUR(ssm::Cursor, 0) // The cursor is updated first

	void init() override;
	void onStart() override;
	void onUpdate() override;

};

} // namespace ssm

#endif // SSM_CURSOR_HPP_INCLUDED
