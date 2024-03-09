#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h" 

namespace nyl 
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
#define NYL_CORE_TRACE(...)    ::nyl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NYL_CORE_INFO(...)     ::nyl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NYL_CORE_WARN(...)     ::nyl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NYL_CORE_ERROR(...)    ::nyl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NYL_CORE_FATAL(...)    ::nyl::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define NYL_TRACE(...)	       ::nyl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NYL_INFO(...)	       ::nyl::Log::GetClientLogger()->info(__VA_ARGS__)
#define NYL_WARN(...)	       ::nyl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NYL_ERROR(...)	       ::nyl::Log::GetClientLogger()->error(__VA_ARGS__)
#define NYL_FATAL(...)	       ::nyl::Log::GetClientLogger()->fatal(__VA_ARGS__)