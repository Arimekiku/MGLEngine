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

		void OnEvent(Event& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Bootstrapper* LoadEngine();
}
