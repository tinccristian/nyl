#include "application.h"

#include "log.h"
#include "core.h"

namespace nyl {

	Application::Application(int& width, int& height, const std::string& title)
		: m_window(std::make_unique<Window>(width,height,title)),Game(std::move(m_window))
	{
		NYL_CORE_TRACE("nyl application constructor");
	}
	Application::~Application()
	{
		NYL_CORE_TRACE("nyl application destructor");
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

    void Application::run()
    {
		Game::init();
		while(!m_window->shouldClose())
		{
			for (Layer* layer: m_LayerStack)
			{
				layer->OnUpdate();
			}
			Game::run();
		}
    }

	void Application::quit()
	{
		NYL_CORE_TRACE("NYL Application quit()");
	}
	
}