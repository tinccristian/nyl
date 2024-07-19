#pragma once

#include "Event.h"
#include "input.h"

namespace nyl {

	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false) : Event(EventType::KeyPressed), m_KeyCode(keycode), m_IsRepeat(isRepeat) {}
		KeyCode GetKeyCode() const { return m_KeyCode; }
		bool IsRepeat() const { return m_IsRepeat; }
		static EventType GetStaticEventType() {return EventType::KeyPressed;}
	private:
		KeyCode m_KeyCode;
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(const KeyCode keycode) : Event(EventType::KeyReleased), m_KeyCode(keycode) {}
		KeyCode GetKeyCode() const { return m_KeyCode; }
		static EventType GetStaticEventType() {return EventType::KeyReleased;}
	private:
		KeyCode m_KeyCode;
	};


	class KeyTypedEvent : public Event
	{
	public:
		KeyTypedEvent(const KeyCode keycode) : Event(EventType::KeyTyped), m_KeyCode(keycode) {}
		KeyCode GetKeyCode() const { return m_KeyCode; }
		static EventType GetStaticEventType() {return EventType::KeyTyped;}
	private:
		KeyCode m_KeyCode;
	};

}
