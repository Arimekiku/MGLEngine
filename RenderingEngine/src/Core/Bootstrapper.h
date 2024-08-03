#pragma once

#include "Window.h"
#include <memory>

namespace RenderingEngine
{
	class Bootstrapper
	{
	public:
		Bootstrapper();
		virtual ~Bootstrapper();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Bootstrapper* LoadEngine();
}
