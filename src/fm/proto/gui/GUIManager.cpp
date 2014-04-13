#include <fm/proto/gui/GUIManager.hpp>
#include <fm/proto/graphics/RenderSystem.hpp>
#include <fm/proto/core/Entity.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void GUIManager::addListener(AComponent* listener)
{
	m_inputListeners.push_back(listener);
}

//------------------------------------------------------------------------------
void GUIManager::removeListener(AComponent* listener)
{
	for(auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it)
	{
		if(*it == listener)
		{
			// TODO optimize me : replace by the last element, then erase the last
			m_inputListeners.erase(it);
			break;
		}
	}
}

//------------------------------------------------------------------------------
bool GUIManager::onEvent(const sf::Event& event)
{
	// For each input listener
	for(auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it)
	{
		AComponent & cmp = **it;
		// Notify of the event
		if(cmp.entity().activeInHierarchy() && cmp.enabled() && cmp.onEvent(event))
		{
			// The event has been consumed
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------
bool f_listenerOrder(const AComponent * const& a, const AComponent * const& b)
{
	if(a->entity().renderer() != nullptr && a->entity().renderer() != nullptr)
	{
		return RenderSystem::getRendererOrder(a->entity().renderer(), b->entity().renderer());
	}
	else
	{
		return a->entity().layer().drawOrder < b->entity().layer().drawOrder;
	}
}

void GUIManager::onUpdate()
{
	std::sort(m_inputListeners.begin(), m_inputListeners.end(), f_listenerOrder);
}

} // namespace zn



