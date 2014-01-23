#ifndef HEADER_ZN_CAMERA_HPP_INCLUDED
#define HEADER_ZN_CAMERA_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <fm/proto/Component.hpp>

namespace zn
{

// Performs a rendering of a specific rectangular area of the scene.
class ZN_API Camera : public AComponent
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

	// TODO ADAPTED_INT ScaleMode that keeps pixels scaled by an integer factor

	Camera();
	~Camera();

	void onAdd(Entity * e) override;
	void onDestroy() override;

	// Sets the size of the rectangular view in world units
	void setSize(const sf::Vector2f & s);

	// Sets the viewport rectangle the camera should render at,
	// with bounds specified in [0, 1] coordinates.
	// For instance, Rect(0,0,0.5,1) is the left part of the screen.
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




