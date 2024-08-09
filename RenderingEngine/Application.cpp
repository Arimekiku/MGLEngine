#define GLFW_INCLUDE_NONE
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "Core/Bootstrapper.h"
#include "Core/Logger/Log.h"

int main(int argc, char** argv)
{
	RenderingEngine::Log::Initialize();

	const auto app = new RenderingEngine::Bootstrapper;
	app->Run();
	delete app;
}
