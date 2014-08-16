/*
Application.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_WINDOWAPPLICATION_HPP_INCLUDED
#define HEADER_ZN_WINDOWAPPLICATION_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <squirrel.h>

#include <fm/asset/AssetBank.hpp>
#include <fm/app/PerformanceGraph.hpp>
#include <fm/app/GameSettings.hpp>
#include <fm/scene/core/Scene.hpp>
#include <fm/reflect/ObjectTypeDatabase.hpp>
#include <fm/app/TimeStepper.hpp>
#include <fm/app/Application.hpp>

namespace zn
{

// Top-level class handling the execution of the game in a window.
// There must be only one Application (or derivated) instance created in the
// whole program execution.
class ZN_API WindowApplication : public Application
{
public:

	// Constructs a Application with an optional title.
	// An empty name means using the title defined in GameSettings.
	WindowApplication(std::string title = "");

	virtual ~WindowApplication();

	// Init and executes the main loop
	void start() override;

	// Tells the game to stop
	void stop() override;

	// Sets wether the game should be in fulscreen or not.
	// Does nothing if the game is already in the given state.
	void setFullScreen(bool fullScreen);

	// Get fullscreen state
	inline bool fullScreen() const { return m_fullScreen; }

	// Returns the final render target of the application (usually the main window)
	inline sf::RenderTarget & renderTarget()
	{
		return m_window;
	}

	virtual sf::Vector2i screenSize() const override;
	virtual sf::Vector2i mousePosition() const override;
	virtual void setSystemCursorVisible(bool visible) override;

protected:

	// Called when an event is received (input or window change)
	// The default implementation does nothing.
	// (some events like app-close or screen resize are handled in the main loop)
	virtual void onEvent(sf::Event & e) {}

	//sf::RenderTexture   m_renderTexture;
	sf::RenderWindow    m_window;
	AssetBank           m_assets;
	Scene               m_scene;
	HSQUIRRELVM         m_squirrelVM;

private:

	bool init();
	void update();
	void render();

	void onScreenResize(sf::Vector2u size);

	GameSettings        m_gameSettings; // startup game settings

	bool                m_fullScreen;
	bool                m_runFlag;
	std::string         m_title;
	sf::View            m_screenView;
	TimeStepper         m_timeStepper;

#ifdef ZN_DEBUG
	sf::Clock           m_profileClock;
	PerformanceGraph    m_updateTimeGraph;
	PerformanceGraph    m_renderTimeGraph;
#endif

};

} // namespace zn

#endif // HEADER_ZN_WINDOWAPPLICATION_HPP_INCLUDED





