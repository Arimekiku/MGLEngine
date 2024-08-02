#include "Core.h"


class Application : public RenderingEngine::Bootstrapper
{
public:
	Application()
	{
	}

	~Application()
	{
	}
};

int main()
{
	RenderingEngine::Log::Initialize();
	RenderingEngine::Log::GetCoreLogger()->trace("Logger works!");

	Application* app = new Application();
	app->Run();
	delete app;
}
