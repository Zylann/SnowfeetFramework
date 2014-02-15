#include <list>

#include <fm/proto/graphics/RenderSystem.hpp>
#include <fm/proto/Entity.hpp>

#ifdef ZN_DEBUG
	// To draw colliders
	#include <fm/proto/physics/Collider.hpp>
#endif

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
bool f_cameraOrder(const Camera *&cam1, const Camera *&cam2)
{
    return cam1->depth < cam2->depth;
}

//------------------------------------------------------------------------------
void RenderSystem::update()
{
	m_cameras.update();
	m_renderers.update();
}

//------------------------------------------------------------------------------
bool f_rendererOrder(const ARenderer *&r1, const ARenderer *&r2)
{
    return r1->drawOrder < r2->drawOrder;
}

//------------------------------------------------------------------------------
void RenderSystem::draw(sf::RenderTarget& finalTarget, sf::RenderStates states) const
{
#ifdef ZN_DEBUG

	sf::Clock profileClock;

	bool drawBounds = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9);
	bool drawColliders = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10);

#endif

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
	    if(camera->entity().activeInHierarchy())
        {
            sortedCameras.push_back(camera);
        }
	}
	sortedCameras.sort(f_cameraOrder);

	// For each active camera
	for(auto cameraIt = sortedCameras.cbegin(); cameraIt != sortedCameras.cend(); ++cameraIt)
	{
		const Camera & camera = **cameraIt;

		sf::RenderTarget & target = *(camera.renderTarget());

		// Set view transform
		target.setView(camera.internalView());
		finalTarget.setView(camera.internalView());

		// Filter and sort renderers by draw order
		std::list<const ARenderer*> drawList;
		for(auto it = m_renderers.cbegin(); it != m_renderers.cend(); ++it)
		{
			const ARenderer * renderer = *it;
			const Entity & entity = renderer->entity();

			// If the entity is active and is on a layer seen by the camera
			if(entity.activeInHierarchy() && ((1 << entity.layer()) & camera.layerMask))
			{
				// Add the entity's renderer to the draw list
				drawList.push_back(renderer);
			}
		}

		// Sort the list by draw order
		drawList.sort(f_rendererOrder);

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
			if(drawBounds)
			{
				// Draw renderer bounds
				// TODO use global bounds when they will be implemented
				sf::FloatRect bounds = renderer.localBounds();
				sf::RectangleShape rect(sf::Vector2f(bounds.width, bounds.height));
				rect.setFillColor(sf::Color::Transparent);
				rect.setOutlineColor(sf::Color(64,64,255));
				rect.setPosition(renderer.entity().transform.position());
				rect.setOutlineThickness(1);
				finalTarget.draw(rect);
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
#endif
		}
	}

#if defined(ZN_DEBUG) && defined(ZN_PROFILE_SCENE)
	float renderTimeMs = profileClock.getElapsedTime().asMilliseconds();
	static u32 renderTimes = 0;
	if((++renderTimes) % 100 == 0)
	{
		std::cout << "D: RenderSystem: render time: " << renderTimeMs << "ms" << std::endl;
	}
#endif
}

} // namespace zn




