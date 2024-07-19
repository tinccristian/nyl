#include "application.h"
#include "log.h"
#include "core.h"
namespace nyl {

	//Application* Application::m_instance = nullptr;

	Application::Application(int& width, int& height, const std::string& title)
		: Game(width, height, title), m_height(height), m_width(width), m_title(title) 
	{
		NYL_CORE_TRACE("nyl application constructor");
	}
	Application::~Application()
	{
		NYL_CORE_TRACE("nyl application destructor");
	}

	void Application::quit()
	{
		NYL_CORE_TRACE("NYL Application quit()");
	}
	
}