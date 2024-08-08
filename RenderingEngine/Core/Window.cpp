#include "mxpch.h"

#include "glad/glad.h"
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
			const int isInitialized = glfwInit();
			LOG_CORE_ASSERT(isInitialized, "GLFW initialize failed")

			s_GLFWInit = true;
		}

		m_Window = glfwCreateWindow(properties.Width, properties.Height, properties.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);

		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		LOG_CORE_ASSERT(status, "GLAD initialize failed")

		glViewport(0, 0, m_Data.Properties.Width, m_Data.Properties.Height);

		glfwSetErrorCallback([](int error, const char* description)
		{
			LOG_CORE_ERROR("GLFW ({0}, {1})", error, description);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* w, const int width, const int height)
		{
			auto& [properties, fun] = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));
			properties.Width = width;
			properties.Height = height;

			WindowResizeEvent e(width, height);
			fun(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* w)
		{
			auto& [_, fun] = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			WindowCloseEvent e;
			fun(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* w, const int key, int code, const int action, int modes)
		{
			auto& [_, fun] = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			if (action == GLFW_PRESS)
			{
				KeyPressedEvent e(key, 0);
				fun(e);
				return;
			}

			if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent e(key);
				fun(e);
				return;
			}

			if (action == GLFW_REPEAT)
			{
				KeyPressedEvent e(key, 1);
				fun(e);
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, const uint32_t keycode)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(keycode);
			data.Callback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* w, const int button, const int action, int modes)
		{
			auto& [_, fun] = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			if (action == GLFW_PRESS)
			{
				MousePressedEvent e(button);
				fun(e);
				return;
			}

			if (action == GLFW_RELEASE)
			{
				MouseReleasedEvent e(button);
				fun(e);
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset)
		{
			auto& [_, fun] = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			fun(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* w, const double xPos, const double yPos)
		{
			auto& [_, fun] = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));

			MouseMovedEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
			fun(e);
		});
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::EveryUpdate() const
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::SetVSync(const bool isEnabled)
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
