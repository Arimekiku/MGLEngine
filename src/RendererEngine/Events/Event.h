#pragma once

#include "RendererEngine/Core.h"

#include <string>
#include <functional>

namespace RenderingEngine
{
	enum class EventType
	{
		Null,

		WindowClose, 
		WindowResize, 
		WindowFocus, 
		WindowRender,

		KeyPressed, 
		KeyReleased, 
		KeyTyped,

		MousePressed, 
		MouseReleased, 
		MouseMoved, 
		MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventApplication = BIT(0),
		EventInput = BIT(1),
		EventKeyboard = BIT(2),
		EventMouse = BIT(3),
		EventMouseButton = BIT(4)
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual int GetCategory() const = 0;

		virtual std::string ToString() const { return "Event"; }

		bool InCategory(const EventCategory category) const { return GetCategory() & category; }
		bool Active = true;
	};

	class EventDispatcher
	{
		template <typename TEvent>
		using EventFunc = std::function<bool(TEvent&)>;

	public:
		EventDispatcher(Event& event): m_Event(event) { }

		template <typename TEvent>
		bool Dispatch(EventFunc<TEvent> func)
		{
			if (m_Event.GetEventType() == TEvent::GetStaticType())
			{
				m_Event.Active |= func(static_cast<TEvent&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
