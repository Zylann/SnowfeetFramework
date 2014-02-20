#ifndef HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED
#define HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED

#include <fm/proto/core/ComponentList.hpp>
#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/graphics/Camera.hpp>

namespace zn
{

/// \brief this subsystem is responsible for the whole rendering.
/// it references every component that deals with graphics (cameras, sprites...),
/// and uses them to compose the final rendering.
class ZN_API RenderSystem
{
public:

	/// \brief this function is called when a new renderer has been created.
	/// Do not call it yourself, it's done automatically by all renderers.
	/// \param renderer: newly created renderer
	void registerRenderer(ARenderer * renderer);

	/// \brief this function is called just before a renderer to be destroyed.
	/// Do not call it yourself, it's done automatically by all renderers.
	/// After that call, the system won't be aware of this renderer.
	/// \param renderer: the renderer that will be destroyed
	void unregisterRenderer(ARenderer * renderer);

	/// \brief this function is called when a new camera has been created.
	/// Do not call it yourself, it's done automatically by all cameras.
	/// \param camera: newly created camera
	void registerCamera(Camera * camera);

	/// \brief this function is called just before a camera to be destroyed.
	/// Do not call it yourself, it's done automatically by all cameras.
	/// After that call, the system won't be aware of this camera.
	/// \param camera: the camera that will be destroyed
	void unregisterCamera(Camera * camera);

	/// \brief called when the screen resolution changes.
	/// calls the same function on some referenced components (as for Cameras).
	/// \param resolution: new screen resolution in pixels
	void onScreenResized(sf::Vector2u resolution);

	/// \brief updates this system. Calls update() on its referenced components.
	void update();

	/// \brief performs the whole rendering, taking into account every component
	/// referenced in the system, in order to compose the final frame on the screen.
	/// \param finalTarget: root render target where the final image will go.
	/// \param states: root render state, usually a default one.
	void draw(sf::RenderTarget & finalTarget, sf::RenderStates states) const;

	/// \brief returns how many objects have been drawn on the last frame,
	/// after every filtering and culling operations.
	/// \return object count
	inline unsigned int drawCount() const { return m_drawCount; }

private:

	/// \brief performs the rendering of one camera.
	/// \param camera: a camera from which the scene is rendered.
	/// \param finalTarget: reference to the final render target (usually the screen)
	/// \param states: render state relative to the camera, usually a default one
	void render(const Camera & camera, sf::RenderTarget & finalTarget, sf::RenderStates states) const;

	/// \brief draws the axis-aligned bounding box of a renderer. Used for debug or editor stuff.
	/// \param renderer: the renderer
	/// \param renderTarget: the target where to draw the bounding box
	void drawRendererBounds(const ARenderer & renderer, sf::RenderTarget & renderTarget) const;

	/// \brief holds a reference to every renderer in the scene
	ComponentList<ARenderer> m_renderers;

	/// \brief holds a reference to every camera in the scene
	ComponentList<Camera> m_cameras;

	/// \brief Counter storing how many object have been drawn on the last frame
	mutable unsigned int m_drawCount;

};

} // namespace zn

#endif // HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED


