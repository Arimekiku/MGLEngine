#pragma once

#include "Logger/Log.h"

extern RenderingEngine::Bootstrapper* RenderingEngine::LoadEngine();

int main(int argc, char** argv)
{
	RenderingEngine::Log::Initialize();

	const auto app = RenderingEngine::LoadEngine();
	app->Run();
	delete app;
}
