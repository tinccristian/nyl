#include "core_application.h"
#include "core_log.h"
#include "core.h"
namespace Nyl {

	//Application* Application::m_instance = nullptr;

	Application::Application(int& width, int& height, const std::string& title)
		: Game(width, height, title), m_height(height), m_width(width), m_title(title) {
		// if (m_instance != nullptr) {
		// 	// Handle the error when m_instance is not null
		// }
		// m_instance = this;

		NYL_CORE_TRACE("Nyl application constructor");
	}
	Application::~Application()
	{
		NYL_CORE_TRACE("Nyl application destructor");
	}
	// Application* Application::get() 
	// {
	// 	return m_instance; 
	// }
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