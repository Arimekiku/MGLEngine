#include "mxpch.h"
#include "glad/glad.h"

#include "Bootstrapper.h"

#include <backends/imgui_impl_opengl3_loader.h>

#include "Events/WindowEvent.h"
#include "Layer/ImGui/GuiLayer.h"
#include "Renderer/RenderBuffer.h"

namespace RenderingEngine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	static GLenum ShaderDataTypeToGLenum(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Bool: return GL_BOOL;
			case ShaderDataType::Int: return GL_INT;
			case ShaderDataType::Int2: return GL_INT;
			case ShaderDataType::Int3: return GL_INT;
			case ShaderDataType::Int4: return GL_INT;
			case ShaderDataType::Float: return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3x3: return GL_FLOAT;
			case ShaderDataType::Mat4x4: return GL_FLOAT;
			default: ;
		}

		LOG_CORE_ASSERT(false, "Can't get shader data type")
		return 0;
	}

	Bootstrapper::Bootstrapper()
	{
		s_Instance = this;

		m_LayerStack = LayerStack();

		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(BIND_FUN(OnEvent));

		AddLayer(new GuiLayer());

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float ver[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		m_VertexBuffer.reset(new VertexBuffer(ver, sizeof(ver)));

		{
			RenderBufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		for (const auto& attr : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
			                      attr.GetElementCount(),
			                      ShaderDataTypeToGLenum(attr.Type),
			                      attr.Normalized ? GL_TRUE : GL_FALSE,
			                      m_VertexBuffer->GetLayout().GetStride(),
			                      reinterpret_cast<const void*>(attr.Offset));
			index++;
		}

		int count = 3;
		uint32_t indices[count] =
		{
			0, 1, 2
		};
		m_IndexBuffer.reset(new IndexBuffer(indices, count));

		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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
		while (m_Running)
		{
			glClearColor(.25f, .25f, .25f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_TestShader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawArrays(GL_TRIANGLES, 0, m_IndexBuffer->GetIndexCount());

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
