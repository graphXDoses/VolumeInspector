#pragma once
#include "Engine/Core/Base.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

namespace MobiusEngine
{

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

#define NGN_CORE_TRACE(...)    ::MobiusEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NGN_CORE_INFO(...)     ::MobiusEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NGN_CORE_WARN(...)     ::MobiusEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NGN_CORE_ERROR(...)    ::MobiusEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NGN_CORE_CRITICAL(...) ::MobiusEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define NGN_TRACE(...)         ::MobiusEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NGN_INFO(...)          ::MobiusEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define NGN_WARN(...)          ::MobiusEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NGN_ERROR(...)         ::MobiusEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define NGN_CRITICAL(...)      ::MobiusEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

