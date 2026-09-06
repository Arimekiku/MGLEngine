#include "Bootstrapper.h"

#include <portable_file_dialogs.h>

#include "EditorLayer.h"
#include "Core/Layer/GuiLayer/GuiLayer.h"

namespace Engine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	Bootstrapper::Bootstrapper()
	{
		LOG_ASSERT(pfd::settings::available(), "Portable File Dialogs are not available on this platform.\n");

		s_Instance = this;

		m_Window = Ptr<Window>::Create();
		m_Layers = std::vector<Layer*>();

		m_GuiLayer = new GuiLayer(m_Window);
		m_Layers.push_back(m_GuiLayer);

		const auto editorLayer = new Editor::EditorLayer(m_Window);
		m_Layers.push_back(editorLayer);

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

			if (m_Minimized)
				continue;

            m_LastFrameTime = static_cast<float>(glfwGetTime());

			const auto deltaTime = Time(m_LastFrameTime - startFrameTime);

			m_GuiLayer->Begin();

			for (const auto& layer : m_Layers)
				layer->OnGuiUpdate();
			
			m_GuiLayer->End();

			for (const auto& layer : m_Layers)
				layer->OnEveryUpdate(deltaTime);

			m_Window->OnEveryUpdate();
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
