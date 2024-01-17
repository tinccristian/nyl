#include "Application.h"
#include "Log.h"

namespace Nyl {

	Application* Application::m_instance = nullptr;

	Application::Application(int& width, int& height, const std::string& title)
		: Game(width, height, title), m_height(height), m_width(width), m_title(title) {
		NASSERT(!m_instance);
		m_instance = this;

		NYL_CORE_TRACE("Nyl application constructor");
	}
	Application::~Application()
	{
		NYL_CORE_TRACE("Nyl application destructor");
	}
	Application* Application::get() 
	{
		return m_instance; 
	}
    void Application::run() 
    {
		Game::run();
    }
	void Application::quit()
	{
		NYL_CORE_TRACE("NYL Application quit()");
	}
}

//GLFW will never free any pointer you provide to it, and you must never free any pointer it provides to you.