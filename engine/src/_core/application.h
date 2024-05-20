#pragma once

#include "game.h"
#include "layer_stack.h"
#include "window.h"

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

		void 		 PushLayer  (Layer* layer);
		void 		 PushOverlay(Layer* overlay);

		void         run();
		void         quit();

	protected:
		std::unique_ptr<Window> m_window;
		LayerStack m_LayerStack;
	};

	//to be defined in client
	Application*     CreateApplication();
}