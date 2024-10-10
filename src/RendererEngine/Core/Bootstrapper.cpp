#include "mxpch.h"
#include "Bootstrapper.h"
#include "chrono"

#include "Logger/Log.h"

#include "RendererEngine/Core/Layer/ImGui/GuiLayer.h"

namespace RenderingEngine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	Bootstrapper::Bootstrapper()
	{
		s_Instance = this;

		m_LayerStack = LayerStack();

		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(BIND_FUNC(OnEvent));
	}

	void Bootstrapper::Run()
	{
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

		while (m_Running)
        {
            auto startFrameTime = static_cast<float>(glfwGetTime());
            for (; static_cast<float>(glfwGetTime()) - startFrameTime <= 0.02f;){}

			m_Window->OnEveryUpdate();
			if (m_Minimized)
				continue;

            m_LastFrameTime = static_cast<float>(glfwGetTime());

			const auto deltaTime = Time(m_LastFrameTime - startFrameTime);

			for (const auto layer : m_LayerStack)
				layer->OnEveryUpdate(deltaTime);

			GuiLayer::Begin();

			for (const auto layer : m_LayerStack)
				layer->OnGuiUpdate();

			GuiLayer::End();
        }
	}

	void Bootstrapper::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FUNC(OnWindowCloseEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_FUNC(OnKeyPressedEvent));

		for (const auto layer : m_LayerStack)
		{
			if (e.Active == false)
				return;

			layer->OnEvent(e);
		}
	}

	void Bootstrapper::AddLayer(Layer* layer)
	{
		m_LayerStack.Push(layer);
		layer->Attach();
	}

	bool Bootstrapper::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		LOG_CORE_INFO("Shutdown");
		LOG_CORE_INFO("{0}", e.ToString());

		m_Running = false;
		return false;
	}

	bool Bootstrapper::OnKeyPressedEvent(const KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_ESCAPE)
		{
			LOG_CORE_INFO("Shutdown");

			m_Running = false;
			return true;
		}

		return false;
	}
}
