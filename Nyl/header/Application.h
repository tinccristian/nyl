//Application.h
#pragma once

#include "Core.h"
#include "Game.h"

//std
#include <string>

namespace Nyl 
{
	class NYL_API Application : public Game
	{
	public:
			      Application(int& width, int& height, const std::string& title);
		virtual   ~Application();
		void      run();
		void      quit();
		virtual void Init()=0;
		virtual void Update()=0;
		virtual void Quit()=0;

	public:
		static Application* get();

	protected:
		int                 m_width, m_height;
		std::  string       m_title;

	private:
		static Application* m_instance;

	};
	//to be defined in client
	Application*     CreateApplication();

}