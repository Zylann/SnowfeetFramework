#ifndef HEADER_DEMO_ROTATE_HPP_INCLUDED
#define HEADER_DEMO_ROTATE_HPP_INCLUDED

#include "fm/scene/behaviour/Behaviour.hpp"

namespace demo
{

class Rotate : public zn::Behaviour
{
public:

	ZN_OBJECT(demo::Rotate, zn::Behaviour)

	s32 getUpdateOrder() const override { return 10; }

	void onUpdate() override
	{
		f32 delta = entity().scene().deltaTime().asSeconds();
		entity().transform.rotate(90.f * delta);
	}

};

} // namespace demo

#endif // HEADER_DEMO_ROTATE_HPP_INCLUDED

