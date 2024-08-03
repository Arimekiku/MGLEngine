#include "mxpch.h"
#include "Bootstrapper.h"

#include "Events/WindowEvent.h"

namespace RenderingEngine
{
	Bootstrapper::Bootstrapper()
	{
		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback([this](auto&& e) { OnEvent(std::forward<decltype(e)>(e)); });
	}

	Bootstrapper::~Bootstrapper()
	{
		
	}

	void Bootstrapper::Run()
	{
		WindowResizeEvent e(1280, 720);
		LOG_CLIENT_TRACE(e.ToString());

		while (m_Running)
		{
			m_Window->EveryUpdate();
		}
	}

	void Bootstrapper::OnEvent(Event& e)
	{
		LOG_CORE_INFO("{0}", e.ToString());
	}
}
