#pragma once

#ifdef NYL_PLATFORM_WINDOWS

extern Nyl::Application* Nyl::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Nyl::CreateApplication();
	app->Run();
	delete app;
}

#endif