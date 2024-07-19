#pragma once

#include "game.h"

#include <string>

namespace nyl 
{
	class NYL_API Application : public Game
	{
	public:
		Application(int& width, int& height, const std::string& title);
		virtual      ~Application();
		virtual void Init()=0;
		virtual void Update(float deltaTime)=0;
		virtual void ProcessInput(float deltaTime) = 0;
		virtual void Quit()=0;
		void         quit();

	protected:
		int                 m_width, m_height;
		std::string         m_title;

	};
	Application*     CreateApplication(); //to be defined in client
}