#ifndef HEADER_DEMO_CAT_HPP_INCLUDED
#define HEADER_DEMO_CAT_HPP_INCLUDED

#include "Character.hpp"

namespace demo
{

class Cat : public Character
{
public:

	ZN_BEHAVIOUR(demo::Cat, 10)

	void onCreate() override;
	void onUpdate() override;

private:

	f32 m_timeBeforeAction;
	u8 m_motorDir;

};

} // namespace demo

#endif // HEADER_DEMO_CAT_HPP_INCLUDED

