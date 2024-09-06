#pragma once

#include "Event.h"

#include <sstream>

namespace RenderingEngine
{
	class WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() = default;

		[[nodiscard]] int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowClose; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] std::string ToString() const override
		{
			return "WindowCloseEvent";
		}
	};

	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(const uint16_t width, const uint16_t height)
		{
			m_Width = width;
			m_Height = height;
		}

		[[nodiscard]] uint16_t GetWidth() const { return m_Width; }
		[[nodiscard]] uint16_t GetHeight() const { return m_Height; }

		[[nodiscard]] int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowResize; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ")";
			return ss.str();
		}

	private:
		uint16_t m_Width, m_Height;
	};

	class WindowFocusEvent final : public Event
	{
	public:
		WindowFocusEvent() = default;

		[[nodiscard]] int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowFocus; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] std::string ToString() const override
		{
			return "WindowFocusEvent";
		}
	};

	class WindowRenderEvent final : public Event
	{
	public:
		WindowRenderEvent() = default;

		[[nodiscard]] int GetCategory() const override { return EventApplication; }
		static EventType GetStaticType() { return EventType::WindowRender; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] std::string ToString() const override
		{
			return "WindowRenderEvent";
		}
	};
}
