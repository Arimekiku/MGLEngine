#include <cstdio>

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
	Application* app = new Application();
	app->Run();
	delete app;
}