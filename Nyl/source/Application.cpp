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
        Window window(800, 800, "Antares");

        if (!window.Init()) 
        {
            NYL_CORE_ERROR("Failed to initialize a window!");
            return;
        }


        while (!window.ShouldClose()) 
        {
            window.Update();
        }

    }
}