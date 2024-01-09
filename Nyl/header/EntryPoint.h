#pragma once

#ifdef NYL_PLATFORM_WINDOWS

#include <stdexcept>
#include "Log.h"

extern Nyl::Application* Nyl::CreateApplication();

int main(int argc, char** argv)
{
	Nyl::Log::Init();
	NYL_CORE_WARN("Initialized Log!");
	
	int a = 5;
	NYL_INFO("Initialized Log! Var= {0}",a);
	auto app = Nyl::CreateApplication();
	try {
		app->Run();
	}
	catch (const std::exception& e) 
	{
		NYL_CORE_ERROR(e.what());
		return EXIT_FAILURE;
	}
	delete app;
}

#endif