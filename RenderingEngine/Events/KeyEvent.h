#pragma once

#include "Event.h"

#include <sstream>

namespace RenderingEngine
{
	class KeyEvent : public Event
	{
	public:
		[[nodiscard]] int GetKeyCode() const { return m_KeyCode; }
		[[nodiscard]] int GetCategory() const override { return EventKeyboard | EventInput; }

	protected:
		explicit KeyEvent(const int keyCode) { m_KeyCode = keyCode; }

		int m_KeyCode;
	};

	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keyCode, const int repeatCount) : KeyEvent(keyCode)
		{
			m_RepeatCount = repeatCount;
		}

		static EventType GetStaticType() { return EventType::KeyPressed; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] int GetRepeatCount() const { return m_RepeatCount; }

		[[nodiscard]] std::string ToString() const override
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
		explicit KeyReleasedEvent(const int keyCode) : KeyEvent(keyCode)
		{
		}

		static EventType GetStaticType() { return EventType::KeyReleased; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class KeyTypedEvent final : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(const int keyCode) : KeyEvent(keyCode)
		{
		}

		static EventType GetStaticType() { return EventType::KeyTyped; }
		[[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
	};
}
