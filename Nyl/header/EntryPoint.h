#pragma once

#ifdef NYL_PLATFORM_WINDOWS

extern Nyl::Application* Nyl::CreateApplication();

int main(int argc, char** argv)
{
	Nyl::Log::Init();
	NYL_CORE_WARN("Initialized Log!");
	
	int a = 5;
	NYL_INFO("Initialized Log! Var= {0}",a);
	auto app = Nyl::CreateApplication();
	app->Run();
	delete app;
}

#endif