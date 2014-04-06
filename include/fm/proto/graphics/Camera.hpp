#ifndef HEADER_ZN_CAMERA_HPP_INCLUDED
#define HEADER_ZN_CAMERA_HPP_INCLUDED

#include <cassert>
#include <SFML/Graphics.hpp>
#include <fm/proto/core/Component.hpp>

namespace zn
{

/// \brief Performs a rendering of a specific rectangular area of the scene.
class ZN_API Camera : public AComponent
{
public:

	ZN_COMPONENT(zn::Camera, CG_CAMERA, 0)

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

	// Sets the size of the rectangular view in world units
	void setSize(const sf::Vector2f & s);

	// Sets the zoom used in FIXED mode
	void setFixedZoom(f32 fixedZoom);

	// Sets the viewport rectangle the camera should render at,
	// with bounds specified in [0, 1] coordinates.
	// For instance, Rect(0,0,0.5,1) is the left part of the screen.
	void setViewport(const sf::FloatRect & r);

	/// \brief sets the render target for this camera.
	/// \param targetTexture: texture where the camera will render.
	/// Passing nullptr means render to screen directly.
	void setRenderTexture(sf::RenderTexture * targetTexture);

	/// \brief gets the render target of this camera.
	/// \return the render texture. Can be nullptr.
	inline sf::RenderTexture * renderTexture() const
	{
		return r_renderTexture;
	}

	void onScreenResized(sf::Vector2u resolution);

	sf::Vector2f screenToWorld(const sf::Vector2i & screenPoint) const;
	sf::Vector2f worldToScreen(const sf::Vector2f & worldPoint) const;

	inline void setInternalView(const sf::View & view) { m_view = view; }
	inline const sf::View & internalView() const { return m_view; };

	void setScaleMode(u8 mode);
	inline u8 scaleMode() const { return m_scaleMode; }

	// TODO pixelPerfect parameter

	void onCreate() override;
	void onUpdate() override;

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

	/// \brief Order of rendering. The smaller the earlier, the greater the latter.
	s32 depth;

	/// \brief Layers the camera can see, as a bitmask.
	/// the first bit means layer 0, the second bit means layer 1... and so on.
	u32 layerMask;

	/// \brief Tells if the camera should clear its render target before rendering
	bool doClear;

	/// \brief Background color used when the render target is cleared
	sf::Color clearColor;

private:

	sf::View m_view;
	u8 m_scaleMode;
	f32 m_fixedZoom;

	sf::RenderTexture * r_renderTexture;

};

} // namespace zn

#endif // HEADER_ZN_CAMERA_HPP_INCLUDED




