#include "Window.h"

#include "Events/Event.h"
#include "Logger/Log.h"

namespace Engine
{
	static bool s_GLFWInit = false;

	Window::Window(const WindowProperties& properties)
	{
		m_Data.Properties = properties;

		LOG_INFO("Creating window {0}, ({1}, {2})", properties.Name, properties.Width, properties.Height);

		if (s_GLFWInit == false)
		{
			const int isInitialized = glfwInit();
			LOG_ASSERT(isInitialized, "GLFW initialize failed")

			s_GLFWInit = true;
		}

		m_Window = glfwCreateWindow(properties.Width, properties.Height, properties.Name, nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);

		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		LOG_ASSERT(status, "GLAD initialize failed")

		glViewport(0, 0, m_Data.Properties.Width, m_Data.Properties.Height);

		glfwSetErrorCallback([](int error, const char* description)
		{
			LOG_ERROR("GLFW ({0}, {1})", error, description);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* w, const int width, const int height)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnWindowSizeCallback(w, width, height);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* w)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnWindowCloseCallback();
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* w, const int key, const int code, const int action, int modes)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnKeyCallback(key, action);
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* w, const uint32_t keycode)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnCharCallback(keycode);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* w, const int button, const int action, int modes)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnMouseButtonCallback(button, action);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* w, const double xOffset, const double yOffset)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnMouseScrollCallback(xOffset, yOffset);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* w, const double xPos, const double yPos)
		{
			const auto self = static_cast<Window*>(glfwGetWindowUserPointer(w));
			if (!self) return;

			self->OnCursorPosCallback(xPos, yPos);
		});
	}

	Window::~Window()
	{
		m_Dispatcher.ClearAll();

		glfwDestroyWindow(m_Window);
	}

	void Window::OnEveryUpdate() const
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

	void Window::OnWindowSizeCallback(GLFWwindow* window, const int width, const int height) const
	{
		auto& [properties] = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		properties.Width = width;
		properties.Height = height;

		const WindowResizeEvent e(width, height);
		m_Dispatcher.Publish(e);
	}

	void Window::OnWindowCloseCallback() const
	{
		constexpr WindowCloseEvent e;
		m_Dispatcher.Publish(e);
	}

	void Window::OnKeyCallback(const int key, const int action) const
	{
		if (action == GLFW_PRESS)
		{
			const KeyPressedEvent e(key, 0);
			m_Dispatcher.Publish(e);
			return;
		}

		if (action == GLFW_RELEASE)
		{
			const KeyReleasedEvent e(key);
			m_Dispatcher.Publish(e);
			return;
		}

		if (action == GLFW_REPEAT)
		{
			const KeyPressedEvent e(key, 1);
			m_Dispatcher.Publish(e);
		}
	}

	void Window::OnCharCallback(const uint32_t keycode) const
	{
		const KeyTypedEvent e(keycode);
		m_Dispatcher.Publish(e);
	}

	void Window::OnMouseButtonCallback(const int button, const int action) const
	{
		if (action == GLFW_PRESS)
		{
			const MousePressedEvent e(button);
			m_Dispatcher.Publish(e);
			return;
		}

		if (action == GLFW_RELEASE)
		{
			const MouseReleasedEvent e(button);
			m_Dispatcher.Publish(e);
		}
	}

	void Window::OnMouseScrollCallback(const double xOffset, const double yOffset) const
	{
		const MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
		m_Dispatcher.Publish(e);
	}

	void Window::OnCursorPosCallback(const double xPos, const double yPos) const
	{
		const MouseMovedEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
		m_Dispatcher.Publish(e);
	}
}
