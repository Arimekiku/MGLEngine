#pragma once

#include "Window.h"
#include "Layer/LayerStack.h"
#include "Events/WindowEvent.h"

#include <memory>

#include "Renderer/Shader.h"

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
		bool OnWindowClose(WindowCloseEvent& e);

		static Bootstrapper* s_Instance;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;

		Ref<Shader> m_Test;
	};

	Bootstrapper* LoadEngine();
}
