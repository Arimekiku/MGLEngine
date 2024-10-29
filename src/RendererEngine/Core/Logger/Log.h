#pragma once

#include <spdlog/spdlog.h>

#ifdef _MSC_VER
#define debugbreak() __debugbreak()
#else
#include <signal.h>
#define debugbreak() raise(SIGTRAP)
#endif

namespace RenderingEngine
{
	class Log
	{
	public:
		static void Initialize();

		static auto& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#define LOG_TRACE(...)       ::RenderingEngine::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)        ::RenderingEngine::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)        ::RenderingEngine::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)       ::RenderingEngine::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_ASSERT(x, ...)   { if(x == false) { LOG_ERROR("Invalid assertion: {0}", __VA_ARGS__); debugbreak(); } }