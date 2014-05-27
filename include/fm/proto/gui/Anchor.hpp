#ifndef HEADER_ZN_ANCHOR_HPP_INCLUDED
#define HEADER_ZN_ANCHOR_HPP_INCLUDED

#include <fm/proto/core/Component.hpp>

namespace zn
{

class ZN_API Anchor : public AComponent
{
public:

	ZN_COMPONENT(zn::Anchor, CG_BEHAVIOUR, 0)

	void onAdd(Entity * e) override;
	void onDestroy() override;

	void onStart() override;

};

} // namespace zn

#endif // HEADER_ZN_ANCHOR_HPP_INCLUDED
