/*
Scene.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>

#include <fm/proto/core/Application.hpp>
#include <fm/util/macros.hpp>
#include <fm/proto/core/ComponentFactory.hpp>
#include <fm/util/Log.hpp>

namespace zn
{

// Global game instance reference
Application * g_instance = nullptr;

//------------------------------------------------------------------------------
Application * Application::instance()
{
	return g_instance;
}

//------------------------------------------------------------------------------
Application::Application(std::string title) :
	m_fullScreen(false),
	m_runFlag(false),
	m_title(title)
{
	ZN_CALL_ONCE;
	g_instance = this;
}

//------------------------------------------------------------------------------
Application::~Application()
{
	g_instance = nullptr;
}

//------------------------------------------------------------------------------
void Application::setFullScreen(bool fullScreen)
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
			log.info() << "switched to fullscreen mode." << log.endl();
		}
		else
		{
			m_window.create(
				m_gameSettings.videoMode,
				m_title,
				sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close,
				contextSettings
			);
			log.info() << "switched to windowed mode." << log.endl();
		}

		m_window.setVerticalSyncEnabled(m_gameSettings.verticalSync);
		m_window.setMouseCursorVisible(m_gameSettings.showSystemCursor);

		onScreenResize(m_window.getSize());
	}
}

//------------------------------------------------------------------------------
bool Application::init()
{
	// Setup log system
	log.openFile("log.txt");

	// Register components in factory
	ComponentFactory::registerEngineComponents();

	// Register native user components
	registerNativeUserComponents(ComponentFactory::get());

	// Read config file
	const std::string settingsFileName = "game_settings.json";
	if(!m_gameSettings.loadFromJSONFile(settingsFileName))
	{
		log.info() << settingsFileName << " was not found, creating a new one." << log.endl();
		m_gameSettings.saveToJSONFile(settingsFileName);
	}
	else
	{
		log.info() << "Reading " << settingsFileName << log.endl();
	}

	// If the hardcoded title is empty, use the one from the settings
	if(m_title.empty())
	{
		m_title = m_gameSettings.title;
	}

	// Create game window
	m_fullScreen = !m_gameSettings.fullscreen; // To make the next line work...
	setFullScreen(m_gameSettings.fullscreen); // Creates the SFML window

	m_assets.setRootFolder(m_gameSettings.assetsRoot);

	// Load or create file associations file
	if(!m_assets.loadFileAssociations(m_gameSettings.assetsRoot + "/file_associations.json", true))
	{
		return false;
	}

	// Load startup assets
	// TODO asynchronous loading of assets with a progressBar
	if(!m_assets.loadFromJSON(m_gameSettings.assetsRoot + "/assets.json"))
	{
		return false;
	}

	// Perform game-specific initialization
	return onInit();
}

//------------------------------------------------------------------------------
void Application::start()
{
	if(!init())
	{
		log.err() << "E: Failed to initialize the game." << log.endl();
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

	log.debug() << "Enter main loop" << log.endl();

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

		// TODO option to disable render-on-each-loop and trigger renders manually
		// This would allow still games or future editor GUI to be more efficient

		render();

		m_renderTimeGraph.pushValue(m_profileClock.getElapsedTime().asSeconds());
#else
		update();
		render();
#endif

		m_frameTime = m_frameClock.restart();
	}

#ifdef ZN_DEBUG
	log.debug() << "Exit main loop" << log.endl();
#endif

	m_window.close();

	onClose();
}

//------------------------------------------------------------------------------
void Application::onScreenResize(sf::Vector2u size)
{
//	if(!m_renderTexture.create(m_window.getSize().x*2, m_window.getSize().y*2))
//	{
//		std::cout << "E: Game2D::onScreenResize: failed to create screen renderTexture." << std::endl;
//	}
	m_screenView.setSize(m_window.getSize().x, m_window.getSize().y);
	m_screenView.setCenter(m_window.getSize().x/2, m_window.getSize().y/2);

	log.info() << "screen resized (" << size.x << ", " << size.y << ")" << log.endl();
	m_scene.onScreenResized(size);
}

//------------------------------------------------------------------------------
void Application::update()
{
	// Clamp frametime above minimal value
	const f32 maximalFrametime = 1.f / 10.f;
	f32 seconds = m_frameTime.asSeconds();
	if(seconds > maximalFrametime)
	{
		seconds = maximalFrametime;
	}

	// Update scene for the current timespan
	m_scene.update(sf::seconds(seconds));
}

//------------------------------------------------------------------------------
void Application::render()
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

	// Note: clearing here is not required anymore since it's done with
	// cameras in the scene
	//m_window.clear(sf::Color(20,20,30));

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
void Application::stop()
{
	m_runFlag = false;
}

} // namespace zn


