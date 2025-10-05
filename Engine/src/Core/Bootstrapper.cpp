#include "Bootstrapper.h"

#include <portable_file_dialogs.h>

#include "EditorLayer.h"
#include "Core/Layer/GuiLayer/GuiLayer.h"
#include "Events/EventDispatcher.h"
#include "Logger/Log.h"

namespace Engine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	Bootstrapper::Bootstrapper()
	{
		LOG_ASSERT(pfd::settings::available(), "Portable File Dialogs are not available on this platform.\n");

		s_Instance = this;

		m_Window = std::make_shared<Window>();
		m_LayerStack = LayerMap();

		const auto guiLayer = std::make_shared<GuiLayer>(m_Window);
		m_LayerStack.Push(guiLayer);

		const auto editorLayer = std::make_shared<EditorLayer>(m_Window);
		m_LayerStack.Push(editorLayer);

		auto& dispatcher = m_Window->GetEventDispatcher();
		dispatcher.Subscribe<WindowCloseEvent>(BIND_FUNC(OnWindowCloseEvent));
		dispatcher.Subscribe<KeyPressedEvent>(BIND_FUNC(OnKeyPressedEvent));
	}

	void Bootstrapper::Run()
	{
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

		while (m_Running)
        {
            const auto startFrameTime = static_cast<float>(glfwGetTime());
            while (static_cast<float>(glfwGetTime()) - startFrameTime <= 0.0167f){}

			m_Window->OnEveryUpdate();
			if (m_Minimized)
				continue;

            m_LastFrameTime = static_cast<float>(glfwGetTime());

			const auto deltaTime = Time(m_LastFrameTime - startFrameTime);

			for (const auto& layer : m_LayerStack | std::views::values)
				layer->OnEveryUpdate(deltaTime);

			const auto& guiLayer = m_LayerStack.Get<GuiLayer>();
			guiLayer->Begin();

			for (const auto& layer : m_LayerStack | std::views::values)
				layer->OnGuiUpdate();

			guiLayer->End();
        }
	}

	void Bootstrapper::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		LOG_WARN("Shutdown");

		m_Running = false;
	}

	void Bootstrapper::OnKeyPressedEvent(const KeyPressedEvent& e)
	{
		if (e.KeyCode != GLFW_KEY_ESCAPE)
			return;

		LOG_WARN("Shutdown");
		m_Running = false;
	}
}
