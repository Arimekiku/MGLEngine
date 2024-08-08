#pragma once

#include "Window.h"
#include "Layer/LayerStack.h"
#include "Events/WindowEvent.h"

#include <memory>

#include "Renderer/Camera.h"
#include "Renderer/RenderBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

		std::shared_ptr<RendereringEngine::Shader> m_TestShader;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Camera> m_Camera;
	};

	Bootstrapper* LoadEngine();
}
