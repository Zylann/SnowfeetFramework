/*
Scene.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include <iostream>
#include "GameApp.hpp"

namespace zn
{

GameApp::GameApp(std::string title) :
	m_fullScreen(false),
	m_runFlag(false),
	m_title(title)
{
}

GameApp::~GameApp()
{
}

void GameApp::setFullScreen(bool fullScreen)
{
	if(fullScreen != m_fullScreen)
	{
		m_fullScreen = fullScreen;

		if(m_fullScreen)
		{
			m_window.create(sf::VideoMode::getDesktopMode(), m_title, sf::Style::Fullscreen);
			std::cout << "I: switched to fullscreen mode." << std::endl;
		}
		else
		{
			m_window.create(sf::VideoMode(800,600), m_title);
			std::cout << "I: switched to windowed mode." << std::endl;
		}

		m_window.setVerticalSyncEnabled(true);

		onScreenResize(m_window.getSize());
	}
}

void GameApp::start()
{
	m_fullScreen = true; // To make the next line work...
	setFullScreen(false); // Creates the SFML window

	onInit();

	m_frameClock.restart();
	m_frameTime = sf::seconds(1.f / 60.f);

#ifdef ZN_DEBUG

	m_updateTimeGraph.setColor(sf::Color(255,127,0,192));
	m_updateTimeGraph.setPosition(sf::Vector2f(8,8));
	m_updateTimeGraph.setValueRange(0.0f, 2.f/60.f);
	m_updateTimeGraph.updateMesh();

	m_renderTimeGraph.setColor(sf::Color(0,255,0,192));
	m_renderTimeGraph.setPosition(sf::Vector2f(8+200+8, 8));
	m_renderTimeGraph.setValueRange(0.0f, 2.f/60.f);
	m_renderTimeGraph.updateMesh();

#endif

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

void GameApp::onScreenResize(sf::Vector2u size)
{
//	if(!_renderTexture.create(m_window.getSize().x, m_window.getSize().y))
//	{
//		std::cout << "E: Game2D::onScreenResize: failed to create screen renderTexture." << std::endl;
//	}
	m_screenView.setSize(m_window.getSize().x, m_window.getSize().y);
	m_screenView.setCenter(m_window.getSize().x/2, m_window.getSize().y/2);

	std::cout << "D: screen resized (" << size.x << ", " << size.y << ")" << std::endl;
	m_scene.onScreenResized(size);
}

void GameApp::update()
{
	m_scene.update(m_frameTime);
}

void GameApp::render()
{
//	m_renderTexture.clear(sf::Color(128,128,128));
//	m_renderTexture.draw(m_scene);
//	m_renderTexture.display();
//
//	sf::Sprite screenSprite(m_renderTexture.getTexture());
//	m_window.clear(sf::Color(64,64,64));
//	m_window.setView(m_screenView);
//	m_window.draw(screenSprite);

	m_window.clear(sf::Color(64,64,64));
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

void GameApp::stop()
{
	m_runFlag = false;
}

} // namespace zn


