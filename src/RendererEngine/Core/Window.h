#pragma once

#include "RendererEngine/Events/Event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RenderingEngine
{
	struct WindowProperties
	{
		const char* Name = "Rendering Engine";
		uint16_t Width = 1600, Height = 900;
		bool VSync = true;

		WindowProperties() = default;
		WindowProperties(const char* name, const uint16_t width, const uint16_t height)
		{
			Name = name;
			Width = width;
			Height = height;
		}
	};

	class Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		Window(const WindowProperties& properties = WindowProperties());
		~Window();

		[[nodiscard]] uint16_t GetWidth() const { return m_Data.Properties.Width; }
		[[nodiscard]] uint16_t GetHeight() const { return m_Data.Properties.Height; }
		[[nodiscard]] bool VSyncEnabled() const { return m_Data.Properties.VSync; }
		[[nodiscard]] GLFWwindow* const& GetNativeWindow() const { return m_Window; }

		void OnEveryUpdate() const;
		void SetEventCallback(const EventCallbackFunc& callback) { m_Data.Callback = callback; }
		void SetVSync(bool isEnabled);

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			WindowProperties Properties;

			EventCallbackFunc Callback;
		};

		WindowData m_Data;
	};
}
