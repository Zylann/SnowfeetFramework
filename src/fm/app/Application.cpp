#include <fm/app/Application.hpp>
#include <fm/util/macros.hpp>

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
Application::Application()
{
	ZN_CALL_ONCE;
	g_instance = this;
}

//------------------------------------------------------------------------------
Application::~Application()
{
	g_instance = nullptr;
}

} // namespace zn


