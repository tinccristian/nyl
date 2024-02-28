#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h" 

namespace Nyl 
{

	class NYL_API Log // /write a function to initialize the logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; } // inline is used to avoid multiple definitions of the same function in different translation units
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

// core log macros
#define NYL_CORE_TRACE(...)    ::Nyl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NYL_CORE_INFO(...)     ::Nyl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NYL_CORE_WARN(...)     ::Nyl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NYL_CORE_ERROR(...)    ::Nyl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NYL_CORE_FATAL(...)    ::Nyl::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define NYL_TRACE(...)	       ::Nyl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NYL_INFO(...)	       ::Nyl::Log::GetClientLogger()->info(__VA_ARGS__)
#define NYL_WARN(...)	       ::Nyl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NYL_ERROR(...)	       ::Nyl::Log::GetClientLogger()->error(__VA_ARGS__)
#define NYL_FATAL(...)	       ::Nyl::Log::GetClientLogger()->fatal(__VA_ARGS__)