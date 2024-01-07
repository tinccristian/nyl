#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Nyl 
{

	class NYL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

//Core log macros
#define NYL_CORE_TRACE(...)    ::Nyl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NYL_CORE_INFO(...)     ::Nyl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NYL_CORE_WARN(...)     ::Nyl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NYL_CORE_ERROR(...)    ::Nyl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NYL_CORE_FATAL(...)    ::Nyl::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define NYL_TRACE(...)	       ::Nyl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NYL_INFO(...)	       ::Nyl::Log::GetClientLogger()->info(__VA_ARGS__)
#define NYL_WARN(...)	       ::Nyl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NYL_ERROR(...)	       ::Nyl::Log::GetClientLogger()->error(__VA_ARGS__)
#define NYL_FATAL(...)	       ::Nyl::Log::GetClientLogger()->fatal(__VA_ARGS__)