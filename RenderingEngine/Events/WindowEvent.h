#pragma once

#include "Event.h"

#include <sstream>

namespace RenderingEngine
{
	class WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() = default;

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowClose; }
		EventType GetEventType() const override { return GetStaticType(); }
	};

	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(const uint16_t width, const uint16_t height)
		{
			m_Width = width;
			m_Height = height;
		}

		uint16_t GetWidth() const { return m_Width; }
		uint16_t GetHeight() const { return m_Height; }

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

	class WindowFocusEvent final : public Event
	{
	public:
		WindowFocusEvent() = default;

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowFocus; }
		EventType GetEventType() const override { return GetStaticType(); }
	};

	class WindowRenderEvent final : public Event
	{
	public:
		WindowRenderEvent() = default;

		int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowRender; }
		EventType GetEventType() const override { return GetStaticType(); }
	};
}
