#ifndef HEADER_ZN_BEHAVIOUR_HPP_INCLUDED
#define HEADER_ZN_BEHAVIOUR_HPP_INCLUDED

#include <fm/proto/Component.hpp>

namespace zn
{

// Base class for user-defined gameplay components
class ABehaviour : public AComponent
{
public:

	virtual void onAdd(Entity * e) override;
	virtual void onDestroy() override;

};

} // namespace zn

#endif // HEADER_ZN_BEHAVIOUR_HPP_INCLUDED


