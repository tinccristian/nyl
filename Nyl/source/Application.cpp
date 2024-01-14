#include "Application.h"
#include "Window.h"
#include "Log.h"

namespace Nyl {

	Application::Application()
	{

	}
	Application::~Application()
	{

	}
    void Application::Run() 
    {
        Window window(800, 600, "Antares");

        if (!window.Init()) 
        {
            NYL_CORE_ERROR("Failed to initialize a window!");
            return;
        }
        window.Update();

    }
}