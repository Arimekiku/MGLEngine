#include "mxpch.h"
#include "glad/glad.h"

#include "Bootstrapper.h"

#include "Events/WindowEvent.h"
#include "glm/gtc/type_ptr.inl"
#include "Layer/ImGui/GuiLayer.h"
#include "Renderer/RenderBuffer.h"
#include "Renderer/Renderer.h"

namespace RenderingEngine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	Bootstrapper::Bootstrapper()
	{
		s_Instance = this;

		m_LayerStack = LayerStack();

		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(BIND_FUN(OnEvent));

		m_Camera.reset(new Camera({0, 0, -2}));

		AddLayer(new GuiLayer());

		m_VertexArray.reset(new VertexArray());

		float ver[5 * 7] = {
			-0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		};
		m_VertexBuffer.reset(new VertexBuffer(ver, sizeof(ver)));

		RenderBufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->SetVertexBuffer(m_VertexBuffer);

		int count = 18;
		uint32_t indices[count] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};
		m_IndexBuffer.reset(new IndexBuffer(indices, count));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_TestShader.reset(new Shader(
			"Resources/Shaders/standart.vert",
			"Resources/Shaders/standart.frag"));
	}

	void Bootstrapper::Run()
	{
		glEnable(GL_DEPTH_TEST);

		Renderer::CreateWorld(m_Camera);

		while (m_Running)
		{
			m_Window->EveryUpdate();

			GuiLayer::Begin();
			for (const auto layer : m_LayerStack)
				layer->EveryUpdate();
			GuiLayer::End();

			Renderer::Clear({.25f, .25f, .25f, 1});
			Renderer::RenderIndexed(m_VertexArray, m_TestShader);
		}
	}

	void Bootstrapper::OnEvent(Event& e)
	{
		LOG_CORE_INFO("{0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FUN(OnWindowClose));
	}

	void Bootstrapper::AddLayer(Layer* layer)
	{
		m_LayerStack.Push(layer);
		layer->Attach();
	}

	bool Bootstrapper::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}
}
