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

		static auto& GetCoreLogger() { return s_RendererLogger; }
		static auto& GetClientLogger() { return s_EditorLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_RendererLogger;
		static std::shared_ptr<spdlog::logger> s_EditorLogger;
	};
}

#define LOG_RENDERER_TRACE(...)       ::RenderingEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_RENDERER_INFO(...)        ::RenderingEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_RENDERER_WARN(...)        ::RenderingEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_RENDERER_ERROR(...)       ::RenderingEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_RENDERER_ASSERT(x, ...)   { if(x == false) { LOG_RENDERER_ERROR("Invalid assertion: {0}", __VA_ARGS__); debugbreak(); } }

#define LOG_EDITOR_TRACE(...)     ::RenderingEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_EDITOR_INFO(...)      ::RenderingEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_EDITOR_WARN(...)      ::RenderingEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_EDITOR_ERROR(...)     ::RenderingEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_EDITOR_ASSERT(x, ...) { if(x == false) { LOG_EDITOR_ERROR("Invalid assertion: {0}", __VA_ARGS__); debugbreak(); } }
