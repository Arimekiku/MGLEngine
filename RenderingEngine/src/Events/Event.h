#pragma once

#include "Core.h"

#include <string>
#include <functional>

namespace RenderingEngine
{
	enum class EventType
	{
		Null = 0,
		WindowClose, WindowResize, WindowFocus, WindowRender,
		KeyPressed, KeyReleased,
		MousePressed, MouseReleased, MouseMoved
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
		virtual EventType GetEventType() const = 0;
		virtual int GetCategory() const = 0;

		const char* GetName() const { return m_Name.c_str(); };
		virtual std::string ToString() const { return GetName(); }

		inline bool InCategory(EventCategory category) { return GetCategory() & category; }

	protected:
		std::string m_Name = NAME_OF(this);
		bool m_Active = true;
	};

	class EventDispatcher
	{
		template <typename TEvent>
		using EventFunc = std::function<bool(TEvent&)>;

	public:
		EventDispatcher(Event& event): m_Event(event)
		{
		}

		template <typename TEvent>
		bool Dispatch(EventFunc<TEvent> func)
		{
			if (m_Event.GetEventType() == TEvent::GetStaticType())
			{
				m_Event.m_Active = func(*const_cast<TEvent*>(&m_Event));
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
