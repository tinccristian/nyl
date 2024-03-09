#pragma once

#include "game.h"

//std
#include <string>

namespace nyl 
{
	class NYL_API Application : public Game
	{
	public:
		Application(int& width, int& height, const std::string& title);
		virtual ~Application();
		virtual void Init()=0;
		virtual void Update(float deltaTime)=0;
		virtual void ProcessInput(float deltaTime) = 0;
		virtual void Quit()=0;

		void      run();
		void      quit();
	public:
		// static Application* get();

	protected:
		int                 m_width, m_height;
		std::string         m_title;

	private:
		// static Application* m_instance;

	};
	Application*     CreateApplication(); //to be defined in client
}