#pragma once
#include "../Core/Base.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

namespace VolumeEngine
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

#define NGN_CORE_TRACE(...)    ::VolumeEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NGN_CORE_INFO(...)     ::VolumeEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NGN_CORE_WARN(...)     ::VolumeEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NGN_CORE_ERROR(...)    ::VolumeEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NGN_CORE_CRITICAL(...) ::VolumeEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define NGN_TRACE(...)         ::VolumeEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NGN_INFO(...)          ::VolumeEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define NGN_WARN(...)          ::VolumeEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NGN_ERROR(...)         ::VolumeEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define NGN_CRITICAL(...)      ::VolumeEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

