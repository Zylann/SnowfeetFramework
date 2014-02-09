#ifndef HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED
#define HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED

#include <fm/proto/BasicComponentSystem.hpp>
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

	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

private:

	BasicComponentSystem<ARenderer> m_renderers;
	BasicComponentSystem<Camera> m_cameras;

};

} // namespace zn

#endif // HEADER_ZN_RENDERSYSTEM_HPP_INCLUDED
