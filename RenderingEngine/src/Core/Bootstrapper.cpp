#include "mxpch.h"
#include "Bootstrapper.h"

namespace RenderingEngine
{
	Bootstrapper::Bootstrapper()
	{
		m_Window = std::make_unique<Window>();
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
}