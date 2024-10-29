#pragma once

#include "Event.h"

#include <sstream>

namespace RenderingEngine
{
	class KeyEvent : public Event
	{
	public:
		uint32_t GetKeyCode() const { return m_KeyCode; }
		int GetCategory() const override { return EventKeyboard | EventInput; }

	protected:
		KeyEvent(const uint32_t keyCode) : m_KeyCode(keyCode) { }

		uint32_t m_KeyCode;
	};

	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(const uint32_t keyCode, const int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) { }

		static EventType GetStaticType() { return EventType::KeyPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << ")";
			return ss.str();
		}

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		KeyReleasedEvent(const uint32_t keyCode)
			: KeyEvent(keyCode) { }

		static EventType GetStaticType() { return EventType::KeyReleased; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class KeyTypedEvent final : public KeyEvent
	{
	public:
		KeyTypedEvent(const uint32_t keyCode)
			: KeyEvent(keyCode) { }

		static EventType GetStaticType() { return EventType::KeyTyped; }
		EventType GetEventType() const override { return GetStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
	};
}
