#pragma once

#include "Core.h"

namespace Nyl 
{
	class NYL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//to be defined in client
	Application* CreateApplication();

}

