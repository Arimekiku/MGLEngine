#include "Bootstrapper.h"
#include "Core/Layer/ImGui/GuiLayer.h"
#include "Events/EventDispatcher.h"
#include "Logger/Log.h"

#include <portable_file_dialogs.h>

namespace Engine
{
	Bootstrapper* Bootstrapper::s_Instance = nullptr;

	Bootstrapper::Bootstrapper()
	{
		LOG_ASSERT(pfd::settings::available(), "Portable File Dialogs are not available on this platform.\n");

		s_Instance = this;
		m_LayerStack = LayerStack();
		m_Window = std::make_unique<Window>();

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

			for (const auto& layer : m_LayerStack)
				layer->OnEveryUpdate(deltaTime);

			GuiLayer::Begin();

			for (const auto& layer : m_LayerStack)
				layer->OnGuiUpdate();

			GuiLayer::End();
        }
	}

	void Bootstrapper::AddLayer(const Ref<Layer>& layer)
	{
		m_LayerStack.Push(layer);
		layer->Attach();
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
