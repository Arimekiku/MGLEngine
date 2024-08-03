#include "mxpch.h"
#include "Window.h"

namespace RenderingEngine
{
	static bool s_GLFWInit = false;

	Window::Window(const WindowData& data)
	{
		m_Data = WindowData(data.Name, data.Width, data.Height);
		LOG_CORE_INFO("Creating window {0}, ({1}, {2})", m_Data.Name, m_Data.Width, m_Data.Height);

		if (s_GLFWInit == false)
		{
			int isInitialized = glfwInit();
			LOG_CORE_ASSERT(isInitialized, "GLFW initialize failed");

			s_GLFWInit = true;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void Window::EveryUpdate()
	{
		if (glfwWindowShouldClose(m_Window) == true)
			return;

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::SetVSync(bool isEnabled)
	{
		m_Data.VSync = isEnabled;

		if (m_Data.VSync == true)
		{
			glfwSwapInterval(1);
			return;
		}

		glfwSwapInterval(1);
	}
}
