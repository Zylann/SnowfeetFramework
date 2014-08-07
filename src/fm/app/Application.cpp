/*
Scene.cpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>
#include <ctime>

#include <fm/app/Application.hpp>
#include <fm/util/macros.hpp>
#include <fm/util/Log.hpp>
#include <fm/scene/register_components.hpp>
#include <fm/squirrel/squirrel_utils.hpp>

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
	if(m_squirrelVM)
	{
		// The VM should have been closed before
		log.warn() << "Late closing of SquirrelVM." << log.endl();
		sq_close(m_squirrelVM);
		m_squirrelVM = nullptr;
	}

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
	// Classic random initialization
	srand(std::time(nullptr));

	// Setup log system
	log.openFile("log.txt");

	// Register components in factory
	registerEngineComponentTypes();

	// Register native user components
	registerNativeUserComponents(ObjectTypeDatabase::get());

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

	// Load static assets
	bool indexAssets = true;
	if(!m_assets.load(m_gameSettings.assetsRoot, indexAssets))
	{
		return false;
	}

	try
	{
		// Initialize Squirrel virtual machine
		m_squirrelVM = sq_open(1024);

		// Register the API to Squirrel
		#ifdef ZN_DEBUG
		log.info() << "Registering framework to Squirrel virtual machine" << log.endl();
		#endif
		zn::squirrel::registerFramework(m_squirrelVM);

		// Compile scripts
		// TODO
		// test
//		Sqrat::Script script(m_squirrelVM);
//		std::string gameScript = m_gameSettings.assetsRoot + "/main.nut";
//		script.CompileFile(gameScript);
//		script.Run();
	}
	catch(Sqrat::Exception ex)
	{
		log.err() << "A Squirrel exception occurred: " << ex.Message() << log.endl();
		sq_close(m_squirrelVM);
		m_squirrelVM = nullptr;
		return false;
	}

	// Configure time stepper (at last, to minimize the "startup lag")
	m_timeStepper.setDeltaRange(sf::seconds(1.f / 70.f), sf::seconds(1.f / 30.f));

	// Perform game-specific initialization
	return onInit();
}

//------------------------------------------------------------------------------
void Application::start()
{
	if(!init())
	{
		log.err() << "Failed to initialize the game." << log.endl();
		return;
	}

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
		m_timeStepper.onBeginFrame();

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
#endif

		update();

#ifdef ZN_DEBUG
		m_updateTimeGraph.pushValue(m_profileClock.getElapsedTime().asSeconds());
		m_profileClock.restart();
#endif

		// TODO option to disable render-on-each-loop and trigger renders manually
		// This would allow still games or future editor GUI to be more efficient

		render();

#ifdef ZN_DEBUG
		m_renderTimeGraph.pushValue(m_profileClock.getElapsedTime().asSeconds());
#endif

		m_timeStepper.onEndFrame();
	}

#ifdef ZN_DEBUG
	log.debug() << "Exit main loop" << log.endl();
#endif

	// Close main window
	m_window.close();

	// Close Squirrel virtual machine
	sq_close(m_squirrelVM);
	m_squirrelVM = nullptr;

	// Game-specific close code
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
	// Update scene for the current timespan
	m_timeStepper.callSteps([this](sf::Time delta){
		m_scene.update(delta);
	});
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


