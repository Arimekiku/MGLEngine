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

RenderingEngine::Bootstrapper* RenderingEngine::LoadEngine()
{
	return new Application();
}
