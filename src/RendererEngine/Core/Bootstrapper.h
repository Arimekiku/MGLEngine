#pragma once

#include "Window.h"

#include "Layer/LayerStack.h"
#include "RendererEngine/Core.h"
#include "RendererEngine/Core/Time.h"

#include "RendererEngine/Events/WindowEvent.h"
#include "RendererEngine/Events/KeyEvent.h"

namespace RenderingEngine
{
	class Bootstrapper
	{
	public:
		Bootstrapper();
		~Bootstrapper() = default;

		void Run();
		void Abort() { m_Running = false; };

		void OnEvent(Event& e);
		void AddLayer(Layer* layer);

		static Bootstrapper& GetInstance() { return *s_Instance; }
		[[nodiscard]] Window& GetWindow() const { return *m_Window; }

	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
		bool OnKeyPressedEvent(const KeyPressedEvent& e);

		static Bootstrapper* s_Instance;
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true, m_Minimized = false;
		float m_LastFrameTime = 0;
	};
}
