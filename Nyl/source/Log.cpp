#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Nyl
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");	//https://github.com/gabime/spdlog/wiki/3.-Custom-formatting

		s_CoreLogger   = spdlog::stdout_color_mt("NYL");
		s_CoreLogger   ->set_level(spdlog::level::info);//ignore trace

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger ->set_level(spdlog::level::trace);

	}
}