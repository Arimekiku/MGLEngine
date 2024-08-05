#pragma once

#include <sstream>

#include "Event.h"

namespace RenderingEngine
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowClose; }
		EventType GetEventType() const override { return GetStaticType(); }
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint16_t width, uint16_t height)
		{
			m_Width = width;
			m_Height = height;
		}

		inline uint16_t GetWidth() const { return m_Width; }
		inline uint16_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": (" << m_Width << ", " << m_Height << ")";
			return ss.str();
		}

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowResize; }
		EventType GetEventType() const override { return GetStaticType(); }

	private:
		uint16_t m_Width, m_Height;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() = default;

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowFocus; }
		EventType GetEventType() const override { return GetStaticType(); }
	};

	class WindowRenderEvent : public Event
	{
	public:
		WindowRenderEvent() = default;

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowRender; }
		EventType GetEventType() const override { return GetStaticType(); }
	};
}
