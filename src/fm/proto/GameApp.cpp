/*
Scene.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>
#include "GameApp.hpp"
#include "../util/macros.hpp"

namespace zn
{

// Global game instance reference
GameApp * g_instance = nullptr;

GameApp * GameApp::instance()
{
	return g_instance;
}

//------------------------------------------------------------------------------
GameApp::GameApp(std::string title) :
	m_fullScreen(false),
	m_runFlag(false),
	m_title(title)
{
	ZN_CALL_ONCE;
	g_instance = this;
}

GameApp::~GameApp()
{
	g_instance = nullptr;
}

//------------------------------------------------------------------------------
void GameApp::setFullScreen(bool fullScreen)
{
	if(fullScreen != m_fullScreen)
	{
		m_fullScreen = fullScreen;

		sf::ContextSettings contextSettings(
			0, // Depth buffer bits
			0, // Stencil buffer bits
			m_gameSettings.antialias
		);

		if(m_fullScreen)
		{
			m_window.create(
				sf::VideoMode::getDesktopMode(),
				m_title,
				sf::Style::Fullscreen, contextSettings
			);
			std::cout << "I: switched to fullscreen mode." << std::endl;
		}
		else
		{
			m_window.create(
				m_gameSettings.videoMode,
				m_title,
				sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close,
				contextSettings
			);
			std::cout << "I: switched to windowed mode." << std::endl;
		}

		m_window.setVerticalSyncEnabled(m_gameSettings.verticalSync);
		m_window.setMouseCursorVisible(m_gameSettings.showSystemCursor);

		onScreenResize(m_window.getSize());
	}
}

//------------------------------------------------------------------------------
bool GameApp::init()
{
	// Register components in factory
	ComponentType::registerEngineComponents();

	// Read config file
	const std::string settingsFileName = "game_settings.json";
	if(!m_gameSettings.loadFromJSONFile(settingsFileName))
	{
		std::cout << "I: " << settingsFileName << " was not found, creating a new one." << std::endl;
		m_gameSettings.saveToJSONFile(settingsFileName);
	}

	// If the hardcoded title is empty, use the one from the settings
	if(m_title.empty())
	{
		m_title = m_gameSettings.title;
	}

	// Create game window
	m_fullScreen = !m_gameSettings.fullscreen; // To make the next line work...
	setFullScreen(m_gameSettings.fullscreen); // Creates the SFML window

	// Load startup assets
	// TODO asynchronous loading of assets with a progressBar
	if(!m_assets.loadFromJSON(m_gameSettings.assetsRoot + "/assets.json"))
	{
		return false;
	}

	return onInit();
}

//------------------------------------------------------------------------------
void GameApp::start()
{
	if(!init())
	{
		std::cout << "E: Failed to initialize the game." << std::endl;
		return;
	}

	m_frameClock.restart();
	m_frameTime = sf::seconds(1.f / 60.f);

#ifdef ZN_DEBUG

	// Initialize update time graph
	m_updateTimeGraph.setColor(sf::Color(255,127,0,192));
	m_updateTimeGraph.setPosition(sf::Vector2f(8,8));
	m_updateTimeGraph.setValueRange(0.0f, 2.f/60.f);
	m_updateTimeGraph.updateMesh();

	// Initialize render time graph
	m_renderTimeGraph.setColor(sf::Color(0,255,0,192));
	m_renderTimeGraph.setPosition(sf::Vector2f(8+200+8, 8));
	m_renderTimeGraph.setValueRange(0.0f, 2.f/60.f);
	m_renderTimeGraph.updateMesh();

#endif

	// Main loop

	m_runFlag = true;
	while(m_runFlag && m_window.isOpen())
	{
		sf::Event e;
		while(m_window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
			{
				stop();
			}
			else
			{
				if(e.type == sf::Event::Resized)
				{
					onScreenResize(sf::Vector2u(e.size.width, e.size.height));
				}

				onEvent(e);
			}
		}

#ifdef ZN_DEBUG
		m_profileClock.restart();

		update();

		m_updateTimeGraph.pushValue(m_profileClock.getElapsedTime().asSeconds());
		m_profileClock.restart();

		render();

		m_renderTimeGraph.pushValue(m_profileClock.getElapsedTime().asSeconds());
#else
		update();
		render();
#endif

		m_frameTime = m_frameClock.restart();
	}

	m_window.close();

	onClose();
}

//------------------------------------------------------------------------------
void GameApp::onScreenResize(sf::Vector2u size)
{
//	if(!m_renderTexture.create(m_window.getSize().x*2, m_window.getSize().y*2))
//	{
//		std::cout << "E: Game2D::onScreenResize: failed to create screen renderTexture." << std::endl;
//	}
	m_screenView.setSize(m_window.getSize().x, m_window.getSize().y);
	m_screenView.setCenter(m_window.getSize().x/2, m_window.getSize().y/2);

	std::cout << "D: screen resized (" << size.x << ", " << size.y << ")" << std::endl;
	m_scene.onScreenResized(size);
}

//------------------------------------------------------------------------------
void GameApp::update()
{
	m_scene.update(m_frameTime);
}

//------------------------------------------------------------------------------
void GameApp::render()
{
//	m_renderTexture.setSmooth(true);
//	m_renderTexture.clear(sf::Color(20,20,30));
//	m_renderTexture.draw(m_scene);
//	m_renderTexture.display();
//
//	sf::Sprite screenSprite(m_renderTexture.getTexture());
//	screenSprite.setScale(1.f/2.f, 1.f/2.f);
//	m_window.clear(sf::Color(64,64,64));
//	m_window.setView(m_screenView);
//	m_window.draw(screenSprite);

	m_window.clear(sf::Color(20,20,30));
	m_window.draw(m_scene);

#ifdef ZN_DEBUG
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3))
	{
		const sf::View & lastView = m_window.getView();
		m_window.setView(m_screenView);

		m_window.draw(m_renderTimeGraph);
		m_window.draw(m_updateTimeGraph);

		m_window.setView(lastView);
	}
#endif

	m_window.display();
}

//------------------------------------------------------------------------------
void GameApp::stop()
{
	m_runFlag = false;
}

} // namespace zn


