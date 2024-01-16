#include "Application.h"
#include "Game.h"
#include "Log.h"

namespace Nyl {

	Application* Application::m_instance = nullptr;

	Application::Application(int& width, int& height, const std::string& title)
		:m_height(height),m_width(width),m_title(title)
	{
		NASSERT(!m_instance);
		m_instance = this;

        NYL_CORE_INFO("Nyl application constructor");
	}
	Application::~Application()
	{
		NYL_CORE_INFO("Nyl application destructor");
	}
	Application* Application::get() 
	{
		return m_instance; 
	}
    void Application::run() 
    {
		Game game(m_width, m_height, m_title);
		Init();
		game.init();
		while (!game.ShouldClose())
		{
			game.update();
			Update();
		}
		Quit();
    }
	void Application::quit()
	{
		NYL_CORE_TRACE("NYL Application quit()");
	}
}

//GLFW will never free any pointer you provide to it, and you must never free any pointer it provides to you.