#pragma once

#ifdef NYL_PLATFORM_WINDOWS

#include <stdexcept>
#include "Log.h"

extern Nyl::Application* Nyl::CreateApplication();

int main(int argc, char** argv)
{
	Nyl::Log::Init();
	NYL_CORE_WARN("<><><><><><><><><><>  NYL  <><><><><><><><><><>");

	auto app = Nyl::CreateApplication();
	try {
		app->run();
	}
	catch (const std::exception& e) 
	{
		NYL_CORE_ERROR(e.what());
		return EXIT_FAILURE;
	}
	delete app;
}

#endif