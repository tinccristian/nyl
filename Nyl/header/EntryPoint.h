// main.cpp
// entry point for Nyl applications on Windows

#pragma once
#include "Application.h"
//#ifdef NYL_PLATFORM_WINDOWS

#include <stdexcept>
#include "Log.h"

// declare the application creation function defined by the user

extern Nyl::Application* Nyl::CreateApplication();

int main(int argc, char** argv)
{
	// initialize and test our logger
	Log::Init();
	NYL_CORE_WARN("<><><><><><><><><><>  NYL  <><><><><><><><><><>");

	auto app = Nyl::CreateApplication();
	try {
		// run the application 
		app->run(); //pretty useless for now; it just calls the run function of Game, and the main game loop is defined there
	}
	catch (const std::exception& e) 
	{
		// handle exceptions and log errors
		NYL_CORE_ERROR(e.what());
		return EXIT_FAILURE;
	}
	delete app;
	// exit the program
	return EXIT_SUCCESS;
}

//#endif