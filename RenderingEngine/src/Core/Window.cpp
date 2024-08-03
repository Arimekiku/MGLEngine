#include "mxpch.h"
#include "Window.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace RenderingEngine
{
	static bool s_GLFWInit = false;

	Window::Window(const WindowProperties& properties)
	{
		m_Data = WindowData();
		m_Data.Properties = properties;

		LOG_CORE_INFO("Creating window {0}, ({1}, {2})", properties.Name, properties.Width, properties.Height);

		if (s_GLFWInit == false)
		{
			int isInitialized = glfwInit();
			LOG_CORE_ASSERT(isInitialized, "GLFW initialize failed")

			glfwSetErrorCallback([](int error, const char* description)
			{
				LOG_CORE_ERROR("GLFW ({0}, {1})", error, description);
			});

			s_GLFWInit = true;
		}

		m_Window = glfwCreateWindow(properties.Width, properties.Height, properties.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* w, int width, int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));
			data.Properties.Width = width;
			data.Properties.Height = height;

			WindowResizeEvent e(width, height);
			data.Callback(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* w)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			WindowCloseEvent e;
			data.Callback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* w, int key, int code, int action, int modes)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			if (action == GLFW_PRESS)
			{
				KeyPressedEvent e(key, 0);
				data.Callback(e);
				return;
			}

			if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent e(key);
				data.Callback(e);
				return;
			}

			if (action == GLFW_REPEAT)
			{
				KeyPressedEvent e(key, 1);
				data.Callback(e);
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* w, int button, int action, int modes)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			if (action == GLFW_PRESS)
			{
				MousePressedEvent e(button);
				data.Callback(e);
				return;
			}

			if (action == GLFW_RELEASE)
			{
				MouseReleasedEvent e(button);
				data.Callback(e);
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* w, double xPos, double yPos)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			MouseMovedEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
			data.Callback(e);
		});
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
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
		m_Data.Properties.VSync = isEnabled;

		if (m_Data.Properties.VSync == true)
		{
			glfwSwapInterval(1);
			return;
		}

		glfwSwapInterval(0);
	}
}
