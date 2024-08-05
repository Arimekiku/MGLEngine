#pragma once

#include "Event.h"

#include <sstream>

namespace RenderingEngine
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		int GetCategory() const override { return EventKeyboard | EventInput; }

	protected:
		KeyEvent(int keyCode) { m_KeyCode = keyCode; }

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode) { m_RepeatCount = repeatCount; }

		static EventType GetStaticType() { return EventType::KeyPressed; }
		EventType GetEventType() const override { return GetStaticType(); }

		inline int GetRepeatCount() { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << m_KeyCode << " (" << m_RepeatCount << ")";
			return ss.str();
		}

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode)
		{
		}

		static EventType GetStaticType() { return EventType::KeyReleased; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << m_KeyCode;
			return ss.str();
		}
	};
}
