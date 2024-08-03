#pragma once

#include "spdlog/spdlog.h"

namespace RenderingEngine
{
	class Log
	{
	public:
		static void Initialize();

		static auto& GetCoreLogger() { return s_CoreLogger; }
		static auto& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define LOG_CORE_TRACE(...)       ::RenderingEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)        ::RenderingEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)        ::RenderingEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)       ::RenderingEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...)       ::RenderingEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define LOG_CORE_ASSERT(x, ...)   { if(x == false) { LOG_CORE_ERROR("Invalid assertion: {0}", __VA_ARGS__); __debugbreak(); } }

#define LOG_CLIENT_TRACE(...)     ::RenderingEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_CLIENT_INFO(...)      ::RenderingEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_CLIENT_WARN(...)      ::RenderingEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_CLIENT_ERROR(...)     ::RenderingEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CLIENT_FATAL(...)     ::RenderingEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define LOG_CLIENT_ASSERT(x, ...) { if(x == false) { LOG_CLIENT_ERROR("Invalid assertion: {0}", __VA_ARGS__); __debugbreak(); } }
