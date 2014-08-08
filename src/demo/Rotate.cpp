#include <fm/scene/core.hpp>
#include "demo/Rotate.hpp"

namespace demo
{

ZN_OBJECT_IMPL(demo::Rotate)

void Rotate::onUpdate()
{
	if(entity().transform())
	{
		zn::f32 delta = entity().scene().deltaTime().asSeconds();
		entity().transform()->rotate(90.f * delta);
	}
}

} // namespace demo

