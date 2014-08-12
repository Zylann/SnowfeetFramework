#include <fm/scene/core/AbstractTransform.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::AbstractTransform)

//------------------------------------------------------------------------------
void AbstractTransform::translate(sf::Vector2f motion)
{
	setPosition(position() + motion);
}

//------------------------------------------------------------------------------
void AbstractTransform::rotate(f32 degrees)
{
	setRotation(rotation() + degrees);
}

//------------------------------------------------------------------------------
void AbstractTransform::scale(sf::Vector2f scaleFactor)
{
	sf::Vector2f oldScale = scale();
	setScale(sf::Vector2f(
		oldScale.x * scaleFactor.x,
		oldScale.y * scaleFactor.y
	));
}

} // namespace zn



