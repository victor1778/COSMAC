#pragma once

#include "COSMAC/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace COSMAC
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define COSMAC_CORE_TRACE(...) ::COSMAC::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define COSMAC_CORE_INFO(...) ::COSMAC::Log::GetCoreLogger()->info(__VA_ARGS__)
#define COSMAC_CORE_WARN(...) ::COSMAC::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define COSMAC_CORE_ERROR(...) ::COSMAC::Log::GetCoreLogger()->error(__VA_ARGS__)
#define COSMAC_CORE_CRITICAL(...) ::COSMAC::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define COSMAC_TRACE(...) ::COSMAC::Log::GetClientLogger()->trace(__VA_ARGS__)
#define COSMAC_INFO(...) ::COSMAC::Log::GetClientLogger()->info(__VA_ARGS__)
#define COSMAC_WARN(...) ::COSMAC::Log::GetClientLogger()->warn(__VA_ARGS__)
#define COSMAC_ERROR(...) ::COSMAC::Log::GetClientLogger()->error(__VA_ARGS__)
#define COSMAC_CRITICAL(...) ::COSMAC::Log::GetClientLogger()->critical(__VA_ARGS__)