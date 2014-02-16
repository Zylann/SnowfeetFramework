#ifndef HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED
#define HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED

#include <fm/proto/ComponentList.hpp>
#include <fm/proto/graphics/Renderer.hpp>
#include <fm/proto/graphics/Camera.hpp>

namespace zn
{

class RenderSystem
{
public:

	void registerRenderer(ARenderer * renderer);
	void unregisterRenderer(ARenderer * renderer);

	void registerCamera(Camera * camera);
	void unregisterCamera(Camera * camera);

	void onScreenResized(sf::Vector2u resolution);

	void update();

	void draw(sf::RenderTarget & finalTarget, sf::RenderStates states) const;

	inline unsigned int drawCount() const { return m_drawCount; }

private:

	void render(const Camera & camera, sf::RenderTarget & finalTarget, sf::RenderStates states) const;

	void drawRendererBounds(const ARenderer & renderer, sf::RenderTarget & renderTarget) const;

	ComponentList<ARenderer> m_renderers;
	ComponentList<Camera> m_cameras;

	// Counter storing how many object have been drawn on the last frame
	mutable unsigned int m_drawCount;

};

} // namespace zn

#endif // HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED
