#pragma once

#include <sstream>

#include "Event.h"

namespace RenderingEngine
{
	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_MouseButton; }

		int GetCategory() const override { return EventMouseButton | EventInput; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << m_MouseButton;
			return ss.str();
		}

	protected:
		MouseButtonEvent(int mouseButton) { m_MouseButton = mouseButton; }

		int m_MouseButton;
	};

	class MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(int button) : MouseButtonEvent(button)
		{
		}

		static EventType GetStaticType() { return EventType::MousePressed; }
		EventType GetEventType() const override { return GetStaticType(); }
	};

	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(int button) : MouseButtonEvent(button)
		{
		}

		static EventType GetStaticType() { return EventType::MouseReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float mouseX, float mouseY)
		{
			m_MouseX = mouseX;
			m_MouseY = mouseY;
		}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		int GetCategory() const override { return EventMouse | EventInput; }
		static EventType GetStaticType() { return EventType::MouseMoved; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": (" << m_MouseX << ", " << m_MouseY << ")";
			return ss.str();
		}

	private:
		float m_MouseX, m_MouseY;
	};
}
