#pragma once

#include "Events/EventDispatcher.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
	struct WindowProperties
	{
		const char* Name = "Renderer";
		int Width = 800, Height = 600;
		bool VSync = true;

		WindowProperties() = default;
		WindowProperties(const char* name, const int width, const int height)
		{
			Name = name;
			Width = width;
			Height = height;
		}
	};

	class Window
	{
	public:
		Window(const WindowProperties& properties = WindowProperties());
		~Window();

		int GetWidth() const { return m_Data.Width; }
		int GetHeight() const { return m_Data.Height; }
		bool VSyncEnabled() const { return m_Data.VSync; }
		GLFWwindow* const& GetNativeWindow() const { return m_Window; }
		EventDispatcher& GetEventDispatcher() { return m_Dispatcher; }

		void OnEveryUpdate() const;
		void SetVSync(bool isEnabled);

	private:
		void OnWindowSizeCallback(GLFWwindow* window, int width, int height) const;
		void OnWindowCloseCallback() const;
		void OnKeyCallback(int key, int action) const;
		void OnCharCallback(uint32_t keycode) const;
		void OnMouseButtonCallback(int button, int action) const;
		void OnMouseScrollCallback(double xOffset, double yOffset) const;
		void OnCursorPosCallback(double xPos, double yPos) const;

		GLFWwindow* m_Window;

		EventDispatcher m_Dispatcher;
		WindowProperties m_Data;
	};
}
