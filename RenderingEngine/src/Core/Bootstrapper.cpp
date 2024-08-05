#include "mxpch.h"
#include "Bootstrapper.h"

#include "Events/WindowEvent.h"

namespace RenderingEngine
{
	Bootstrapper::Bootstrapper()
	{
		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback([this](auto&& e)
		{
			OnEvent(std::forward<decltype(e)>(e));
		});
	}

	Bootstrapper::~Bootstrapper()
	{
		
	}

	void Bootstrapper::Run()
	{
		while (m_Running)
		{
			m_Window->EveryUpdate();
		}
	}

	void Bootstrapper::OnEvent(Event& e)
	{
		LOG_CORE_INFO("{0}", e.ToString());

		if (e.GetEventType() == EventType::WindowClose)
			m_Running = false;
	}
}
