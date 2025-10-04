#pragma once

#include "Core.h"

namespace Engine
{
	struct KeyPressedEvent
	{
		KeyPressedEvent(const uint32_t keycode, const int repeatCount)
			: KeyCode(keycode), RepeatCount(repeatCount) { }

		uint32_t KeyCode;
		int RepeatCount;
	};

	struct KeyTypedEvent
	{
		KeyTypedEvent(const uint32_t keycode)
			: KeyCode(keycode) { }

		uint32_t KeyCode;
	};

	struct KeyReleasedEvent
	{
		KeyReleasedEvent(const uint32_t keycode)
			: KeyCode(keycode) { }

		uint32_t KeyCode;
	};

	struct MousePressedEvent
	{
		MousePressedEvent(const int button)
			: MouseButton(button) { }

		int MouseButton;
	};

	struct MouseReleasedEvent
	{
		MouseReleasedEvent(const int button)
			: MouseButton(button) { }

		int MouseButton;
	};

	struct MouseMovedEvent
	{
		MouseMovedEvent(const float x, const float y)
			: MouseX(x), MouseY(y) { }

		float MouseX, MouseY;
	};
	 
	struct MouseScrolledEvent
	{
		MouseScrolledEvent(const float offsetX, const float offsetY)
			: OffsetX(offsetX), OffsetY(offsetY) { }

		float OffsetX, OffsetY;
	};

	struct WindowCloseEvent
	{
		WindowCloseEvent() = default;
	};

	struct WindowResizeEvent
	{
		WindowResizeEvent(const int width, const int height)
			: Width(width), Height(height) { }

		int Width, Height;
	};

	struct WindowFocusEvent
	{
		WindowFocusEvent() = default;
	};
}
