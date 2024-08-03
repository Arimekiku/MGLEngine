#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace RenderingEngine
{
	struct WindowData
	{
		std::string Name;
		uint16_t Width, Height;
		bool VSync;

		explicit WindowData(const std::string& name = "Rendering Engine",
		                    uint16_t width = 800,
		                    uint16_t height = 600)
		{
			Name = name;
			Width = width;
			Height = height;
			VSync = true;
		}
	};

	class Window
	{
	public:
		Window(const WindowData& data = WindowData());
		~Window();

		auto GetWidth() const { return m_Data.Width; } 
		auto GetHeight() const { return m_Data.Height; }
		auto VSyncEnabled() const { return m_Data.VSync; }

		void EveryUpdate();
		void SetVSync(bool isEnabled);

	private:
		GLFWwindow* m_Window;

		WindowData m_Data;
	};
}
