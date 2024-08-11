#pragma once

#include "Window.h"
#include "Layer/LayerStack.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"

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
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
		bool OnKeyPressedEvent(const KeyPressedEvent& e);

		static Bootstrapper* s_Instance;
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;
	};

	Bootstrapper* LoadEngine();
}
