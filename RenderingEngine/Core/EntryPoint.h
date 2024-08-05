#pragma once

#define GLFW_INCLUDE_NONE

#include "Bootstrapper.h"
#include "Logger/Log.h"

extern RenderingEngine::Bootstrapper* RenderingEngine::LoadEngine();

int main(int argc, char** argv)
{
	RenderingEngine::Log::Initialize();

	const auto app = RenderingEngine::LoadEngine();
	app->Run();
	delete app;
}
