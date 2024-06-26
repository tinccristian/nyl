// entry point for nyl applications on Windows

#pragma once
#include "log.h"
#include "application.h"

#include <stdexcept>

// declare the application creation function defined by the user

extern nyl::Application* nyl::CreateApplication();

int main(int argc, char** argv)
{
	// initialize and test our logger
	nyl::Log::Init();
	NYL_CORE_WARN("<><><><><><><><><><>  NYL  <><><><><><><><><><>");

	auto app = nyl::CreateApplication();
	try {
		// run the application 
		app->run(); //pretty useless layer for now; it just calls the run function of Game, and the main game loop is defined there
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