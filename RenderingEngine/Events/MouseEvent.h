#pragma once

#include "Event.h"

#include <sstream>

namespace RenderingEngine
{
	class MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const { return m_MouseButton; }
		int GetCategory() const override { return EventMouseButton | EventInput; }

	protected:
		explicit MouseButtonEvent(const int mouseButton) { m_MouseButton = mouseButton; }

		int m_MouseButton;
	};

	class MousePressedEvent final : public MouseButtonEvent
	{
	public:
		explicit MousePressedEvent(const int button) : MouseButtonEvent(button)
		{
		}

		static EventType GetStaticType() { return EventType::MousePressed; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressedEvent: " << m_MouseButton;
			return ss.str();
		}
	};

	class MouseReleasedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseReleasedEvent(const int button) : MouseButtonEvent(button)
		{
		}

		static EventType GetStaticType() { return EventType::MouseReleased; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << m_MouseButton;
			return ss.str();
		}
	};

	class MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(const float mouseX, const float mouseY)
		{
			m_MouseX = mouseX;
			m_MouseY = mouseY;
		}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		int GetCategory() const override { return EventMouse | EventInput; }
		static EventType GetStaticType() { return EventType::MouseMoved; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_MouseX << ", " << m_MouseY << ")";
			return ss.str();
		}

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
		{
			m_XOffset = xOffset;
			m_YOffset = yOffset;
		}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		int GetCategory() const override { return EventMouse | EventInput; }
		static EventType GetStaticType() { return EventType::MouseScrolled; }
		EventType GetEventType() const override { return GetStaticType(); }

	private:
		float m_XOffset, m_YOffset;
	};
}
