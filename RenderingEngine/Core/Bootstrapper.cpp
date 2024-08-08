#include "mxpch.h"
#include "glad/glad.h"

#include "Bootstrapper.h"

#include "Events/WindowEvent.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
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

		m_Camera.reset(new Camera({0.0f, 0.0f, -2.0f}));

		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 camMatrix;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = camMatrix * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_TestShader.reset(new RendereringEngine::Shader(vertexSrc, fragmentSrc));
	}

	void Bootstrapper::Run()
	{
		glEnable(GL_DEPTH_TEST);

		while (m_Running)
		{
			Renderer::Clear({.25f, .25f, .25f, 1});

			m_TestShader->Bind();

			m_Camera->SetProjection(45.0f, 0.1f, m_TestShader, 100.0f, "camMatrix");
			Renderer::RenderIndexed(m_VertexArray);

			for (const auto layer : m_LayerStack)
				layer->EveryUpdate();

			m_Window->EveryUpdate();
		}
	}

	void Bootstrapper::OnEvent(Event& e)
	{
		LOG_CORE_INFO("{0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FUN(OnWindowClose));

		for (const auto& layer : m_LayerStack)
		{
			if (e.Active == false)
				break;

			layer->OnEvent(e);
		}
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
