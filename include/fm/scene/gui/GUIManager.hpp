#ifndef HEADER_ZN_GUIMANAGER_HPP_INCLUDED
#define HEADER_ZN_GUIMANAGER_HPP_INCLUDED

#include <unordered_set>
#include <fm/sfml/InputListener.hpp>
#include <fm/scene/core/Component.hpp>

namespace zn
{

class GUIManager : public IInputListener
{
public:

	void addListener(Component * listener);
	void removeListener(Component * listener);

	bool onEvent(const sf::Event & event) override;

	void onUpdate();

private:

	std::vector<Component*> m_inputListeners;

};

} // namespace zn

#endif // HEADER_ZN_GUIMANAGER_HPP_INCLUDED

