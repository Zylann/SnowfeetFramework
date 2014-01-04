#ifndef HEADER_ZN_CAMERA_HPP_INCLUDED
#define HEADER_ZN_CAMERA_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Component.hpp"

namespace zn
{

class Camera : public AComponent
{
public:

	ZN_COMPONENT(zn::Camera, CG_CAMERA)

	// Which method should be used when the screen's resolution changes
	enum ScaleMode
	{
		STRETCHED=0, // Keep the same size, but deforms if the resolution changes.
		FIXED, // Keep same zoom but change size
		ADAPTED // Keep same width but change the ratio
	};

	Camera();
	~Camera();

	void onAdd(Entity * e) override;
	void onDestroy() override;

	void setSize(const sf::Vector2f & s);
	void setViewport(const sf::FloatRect & r);

	void onScreenResized(sf::Vector2u resolution);

	inline void setInternalView(const sf::View & view) { m_view = view; }
	inline const sf::View & internalView() const { return m_view; };

	void setScaleMode(u8 mode);
	inline u8 scaleMode() const { return m_scaleMode; }

	// TODO pixelPerfect parameter

	void init() override;
	void onUpdate() override;

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

	// Order of rendering
	s32 depth = 0;

private:

	sf::View m_view;
	u8 m_scaleMode;

};

} // namespace zn

#endif // HEADER_ZN_CAMERA_HPP_INCLUDED




