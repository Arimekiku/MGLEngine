#include "mxpch.h"
#include "glad/glad.h"

#include "Bootstrapper.h"

#include "imgui.h"
#include "Events/WindowEvent.h"
#include "Layer/ImGui/GuiLayer.h"
#include "Layer/Scene/SceneLayer.h"
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
		AddLayer(new SceneLayer());
	}

	void Bootstrapper::Run()
	{
		glEnable(GL_DEPTH_TEST);

		while (m_Running)
		{
			m_Window->EveryUpdate();

			GuiLayer::Begin();

			for (const auto layer : m_LayerStack)
				layer->EveryUpdate();

			GuiLayer::End();
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
