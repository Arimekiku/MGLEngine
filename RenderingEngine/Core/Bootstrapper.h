#pragma once

#include "Window.h"
#include "LayerStack.h"

#include <memory>

namespace RenderingEngine
{
	class Bootstrapper
	{
	public:
		Bootstrapper();
		~Bootstrapper() = default;

		void Run();

		void OnEvent(Event& e);
		void AddLayer(Layer* layer);

		static const Bootstrapper& GetInstance() { return *s_Instance; }
		Window& GetWindow() const { return *m_Window; }

	private:
		static Bootstrapper* s_Instance;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;
	};

	Bootstrapper* LoadEngine();
}
