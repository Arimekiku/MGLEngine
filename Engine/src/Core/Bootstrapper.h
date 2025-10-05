#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/EventDispatcher.h"
#include "Layer/LayerMap.h"

namespace Engine
{
	class Bootstrapper
	{
	public:
		Bootstrapper();
		~Bootstrapper() = default;

		void Run();
		void Abort() { m_Running = false; }

		static Bootstrapper& GetInstance() { return *s_Instance; }
		EventDispatcher& GetEventDispatcher() { return m_Window->GetEventDispatcher(); }
		Window& GetWindow() const { return *m_Window; }

	private:
		void OnWindowCloseEvent(const WindowCloseEvent& e);
		void OnKeyPressedEvent(const KeyPressedEvent& e);

		static Bootstrapper* s_Instance;
		Ref<Window> m_Window;
		LayerMap m_LayerStack;
		bool m_Running = true, m_Minimized = false;
		float m_LastFrameTime = 0;
	};
}
