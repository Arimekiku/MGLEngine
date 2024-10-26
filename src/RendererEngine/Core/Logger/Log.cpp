#include "mxpch.h"

#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace RenderingEngine
{
	std::shared_ptr<spdlog::logger> Log::s_RendererLogger;
	std::shared_ptr<spdlog::logger> Log::s_EditorLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_RendererLogger = spdlog::stdout_color_mt("RENDERER");
		s_RendererLogger->set_level(spdlog::level::trace);

		s_EditorLogger = spdlog::stdout_color_mt("EDITOR");
		s_EditorLogger->set_level(spdlog::level::trace);
	}
}
