#include <list>

#include <fm/scene/graphics/RenderSystem.hpp>
#include <fm/scene/core/Entity.hpp>
#include <fm/sfml/sfml2_utils.hpp>

#ifdef ZN_DEBUG
	// To draw colliders
	#include <fm/scene/physics/Collider.hpp>
#endif

#define ZN_PROFILE_RENDERING

namespace zn
{

//------------------------------------------------------------------------------
void RenderSystem::registerRenderer(ARenderer* renderer)
{
	assert(renderer != nullptr);
	m_renderers.add(renderer);
}

//------------------------------------------------------------------------------
void RenderSystem::unregisterRenderer(ARenderer* renderer)
{
	m_renderers.remove(renderer);
}

//------------------------------------------------------------------------------
void RenderSystem::registerCamera(Camera* camera)
{
	assert(camera != nullptr);
	m_cameras.add(camera);
}

//------------------------------------------------------------------------------
void RenderSystem::unregisterCamera(Camera* camera)
{
	m_cameras.remove(camera);
}

//------------------------------------------------------------------------------
void RenderSystem::onScreenResized(sf::Vector2u resolution)
{
	for(auto it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		(*it)->onScreenResized(resolution);
	}
}

//------------------------------------------------------------------------------
void RenderSystem::update()
{
	m_cameras.update();
	m_renderers.update();
}

//------------------------------------------------------------------------------
bool f_cameraOrder(const Camera *&cam1, const Camera *&cam2)
{
    return cam1->depth < cam2->depth;
}

//------------------------------------------------------------------------------
// Static
bool RenderSystem::getRendererOrder(const ARenderer * const & r1, const ARenderer * const & r2)
{
	s32 layerOrder1 = r1->entity().layer().drawOrder;
	s32 layerOrder2 = r2->entity().layer().drawOrder;

	if(layerOrder1 == layerOrder2)
	{
		return r1->drawOrder < r2->drawOrder;
	}
	else
	{
		return layerOrder1 < layerOrder2;
	}
}

//------------------------------------------------------------------------------
void RenderSystem::draw(sf::RenderTarget & finalTarget, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	sf::Clock profileClock;
#endif

	m_drawCount = 0;

	// If there is no camera to render the scene, there is nothing to draw
	if(m_cameras.empty())
	{
#ifdef ZN_DEBUG
		static bool once = true;
		if(once)
		{
			once = false;
			std::cout << "W: RenderSystem::draw: no camera in the scene, nothing to render." << std::endl;
		}
#endif
		return;
	}

	// Filter and sort cameras by depth
	std::list<const Camera*> sortedCameras;
	for(auto cameraIt = m_cameras.cbegin(); cameraIt != m_cameras.cend(); ++cameraIt)
	{
	    const Camera * camera = *cameraIt;
	    if(camera->enabled() && camera->entity().activeInHierarchy())
        {
            sortedCameras.push_back(camera);
        }
	}
	sortedCameras.sort(f_cameraOrder);

	// For each active camera
	for(auto cameraIt = sortedCameras.cbegin(); cameraIt != sortedCameras.cend(); ++cameraIt)
	{
		r_activeCamera = *cameraIt;
		render(*r_activeCamera, finalTarget, states);
	}

	r_activeCamera = nullptr;

#if defined(ZN_DEBUG) && defined(ZN_PROFILE_RENDERING)
	f32 renderTimeMs = profileClock.getElapsedTime().asMilliseconds();
	static u32 renderTimes = 0;
	if((++renderTimes) % 100 == 0)
	{
		std::cout << "D: RenderSystem: " << renderTimeMs << "ms "
			"(" << m_drawCount << " objects)" << std::endl;
	}
#endif
}

//------------------------------------------------------------------------------
void RenderSystem::render(const Camera & camera, sf::RenderTarget & finalTarget, sf::RenderStates states) const
{
#ifdef ZN_DEBUG
	bool drawBounds = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9);
	bool drawColliders = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10);
#endif

	sf::RenderTexture * renderTexture = camera.renderTexture();

	const sf::View & view = camera.internalView();

	// If the camera has a RenderTexture target
	if(renderTexture != nullptr)
	{
		if(camera.doClear)
		{
			// Clear texture
			renderTexture->clear(camera.clearColor);
		}
		// Set view transform on that texture
		renderTexture->setView(view);
	}
	else
	{
		if(camera.doClear)
		{
			finalTarget.clear(camera.clearColor);
		}
	}

	// Set view transform on screen
	finalTarget.setView(view);

	sf::FloatRect viewBounds(view.getCenter()-view.getSize()*0.5f, view.getSize());

	sf::Transform trans;
	trans.rotate(camera.entity().transform.rotation(), view.getCenter());
	viewBounds = trans.transformRect(viewBounds);

	// Filter and sort renderers by draw order
	std::vector<const ARenderer*> drawList;
	for(auto it = m_renderers.cbegin(); it != m_renderers.cend(); ++it)
	{
		const ARenderer * renderer = *it;
		const Entity & entity = renderer->entity();

		// If the renderer is enabled, entity is active and is on a layer seen by the camera
		if(renderer->enabled()
			&& entity.activeInHierarchy()
			&& ((1 << entity.layerIndex()) & camera.layerMask))
		{
			sf::FloatRect entityBounds = renderer->bounds();

			// If the entity's renderer bounds intersect the view bounds
			if(intersects(entityBounds, viewBounds))
			{
				// Add the entity's renderer to the draw list
				drawList.push_back(renderer);
			}
		}
	}

	// Sort the list by draw order
	std::sort(drawList.begin(), drawList.end(), getRendererOrder);

	// Select the right render target
	sf::RenderTarget & target = renderTexture != nullptr ? *renderTexture : finalTarget;

	// Draw filtered renderers in the right order
	for(auto it = drawList.cbegin(); it != drawList.cend(); ++it)
	{
		const ARenderer & renderer = **it;

		// Apply material if any
		Material * material = renderer.material();
		if(material != nullptr)
		{
			material->apply(states);
		}
		else
		{
			states.shader = nullptr;
		}

		// Draw renderer
		target.draw(renderer, states);

#ifdef ZN_DEBUG
		// TODO draw gizmos and handles properly in a separate function
		if(renderTexture == nullptr)
		{
			if(drawBounds)
			{
				drawRendererBounds(renderer, finalTarget);
			}

			if(drawColliders)
			{
				// Draw collider boundaries
				const ACollider * collider = (*it)->entity().collider();
				if(collider != nullptr)
				{
					collider->debug_draw(finalTarget);
				}
			}
		}
#endif
	}

	// After draw, update render texture if any
	if(renderTexture != nullptr)
	{
		renderTexture->display();
	}

	m_drawCount += drawList.size();
}

//------------------------------------------------------------------------------
void RenderSystem::drawRendererBounds(const ARenderer & renderer, sf::RenderTarget & renderTarget) const
{
	sf::FloatRect bounds = renderer.bounds();

	const sf::Color color(64, 64, 255);

	sf::VertexArray rect;

	rect.setPrimitiveType(sf::PrimitiveType::LinesStrip);

	rect.append(sf::Vertex(sf::Vector2f(bounds.left, bounds.top), color));
	rect.append(sf::Vertex(sf::Vector2f(bounds.left+bounds.width, bounds.top), color));
	rect.append(sf::Vertex(sf::Vector2f(bounds.left+bounds.width, bounds.top+bounds.height), color));
	rect.append(sf::Vertex(sf::Vector2f(bounds.left, bounds.top+bounds.height), color));
	rect.append(sf::Vertex(sf::Vector2f(bounds.left, bounds.top), color));

	renderTarget.draw(rect);
}

} // namespace zn




