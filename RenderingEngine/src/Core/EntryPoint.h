#pragma once

#ifdef APP_PLATFORM_WINDOWS

extern RenderingEngine::Bootstrapper* RenderingEngine::LoadEngine();

int main(int argc, char** argv)
{
	RenderingEngine::Log::Initialize();
	RenderingEngine::Log::GetCoreLogger()->trace("Logger works!");

	const auto app = RenderingEngine::LoadEngine();
	app->Run();
	delete app;
}

#endif
