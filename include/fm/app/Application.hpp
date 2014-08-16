#ifndef HEADER_ZN_APPLICATION_HPP_INCLUDED
#define HEADER_ZN_APPLICATION_HPP_INCLUDED

#include <iostream>
#include <exception>

#include <fm/reflect/ObjectTypeDatabase.hpp>

#include <SFML/System.hpp>

namespace zn
{

/// \brief Top-level class handling the execution of a game.
/// There must be only one Application (or derivated) instance created in the
/// whole program execution.
/// Note: an application can have no window (example: a CLI game server).
class ZN_API Application
{
public:

	//--------------------------------------
	// Static members
	//--------------------------------------

	/// \brief Gets the current application instance
	static Application * instance();

	/// \brief Convenience routine for running an Application
	template <class Application_T>
	static int run()
	{
		std::cout << "D: Enter run()" << std::endl;

		int exitCode = 0;

		try
		{
			Application_T app;
			app.start();
		}
		catch(std::exception & e)
		{
			std::cout << "E: Fatal exception: " << e.what() << std::endl;
			std::cout << "The game will close. Press any key to dismiss..." << std::endl;
			getchar();
			exitCode = -1;
		}

		std::cout << "D: Exit run()" << std::endl;
		return exitCode;
	}

	//--------------------------------------
	// Lifecycle public methods
	//--------------------------------------

	virtual ~Application();

	/// \brief Starts the application. It may be blocking.
	virtual void start() = 0;

	/// \brief Asks to shutdown the application cleanly.
	virtual void stop() = 0;

	//--------------------------------------
	// GUI related methods
	//--------------------------------------

	/// \brief Gets the size of the drawable area of the window in pixels, if available
	virtual sf::Vector2i screenSize() const { return sf::Vector2i(0,0); }

	/// \brief Gets mouse position relative to the window in pixels, if available
	virtual sf::Vector2i mousePosition() const { return sf::Vector2i(0,0); }

	/// \brief Hides or shows system's mouse cursor, if available
	virtual void setSystemCursorVisible(bool visible) {}

protected:

	Application();

	/// \brief Called when the application starts, after window's creation, before main loop.
	/// The default implementation does nothing.
	/// \return true on success, false on failure
	virtual bool onInit() { return true; }

	/// \brief Called to register classes that need reflection
	virtual void registerNativeUserClasses(ObjectTypeDatabase & odb) = 0;

	/// \brief Called before the application closes.
	/// The default implementation does nothing.
	virtual void onClose() {}

};

} // namespace zn

#endif // HEADER_ZN_APPLICATION_HPP_INCLUDED


