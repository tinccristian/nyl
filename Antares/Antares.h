#pragma once

#include <Nyl.h>

namespace Antares
{
	class Antares : public Nyl::Application
	{
	public:
		Antares(int width, int height, const std::string& title);
		~Antares();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Quit() override;

	};
}