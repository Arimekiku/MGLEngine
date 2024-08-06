#include "mxpch.h"
#include "Bootstrapper.h"

#include "Input.h"
#include "Events/WindowEvent.h"
#include "ImGui/GuiLayer.h"

namespace RenderingEngine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	Bootstrapper::Bootstrapper()
	{
		s_Instance = this;

		m_LayerStack = LayerStack();

		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback([this](auto&& e)
		{
			OnEvent(std::forward<decltype(e)>(e));
		});

		AddLayer(new GuiLayer());
	}

	void Bootstrapper::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			float x = Input::GetMouseX();
			float y = Input::GetMouseY();
			LOG_CORE_INFO("{0}, {1}", x, y);

			for (const auto layer : m_LayerStack)
				layer->EveryUpdate();

			m_Window->EveryUpdate();
		}
	}

	void Bootstrapper::OnEvent(Event& e)
	{
		LOG_CORE_INFO("{0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>([this](auto&& _)
		{
			m_Running = false;
			return true;
		});

		if (e.GetEventType() == EventType::WindowClose)
			m_Running = false;

		for (const auto& layer : m_LayerStack)
		{
			if (e.Active)
				break;

			layer->OnEvent(e);
		}
	}

	void Bootstrapper::AddLayer(Layer* layer)
	{
		m_LayerStack.Push(layer);
		layer->Attach();
	}
}
