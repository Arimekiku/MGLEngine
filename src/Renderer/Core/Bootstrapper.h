#pragma once

#include "Window.h"
#include "Layer/LayerStack.h"
#include "Renderer/Core.h"
#include "Renderer/Events/WindowEvent.h"
#include "Renderer/Events/KeyEvent.h"

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
		[[nodiscard]] Window& GetWindow() const { return *m_Window; }

	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
		bool OnKeyPressedEvent(const KeyPressedEvent& e);

		static Bootstrapper* s_Instance;
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true, m_Minimized = false;
	};

	Bootstrapper* LoadEngine();
}
