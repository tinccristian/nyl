#pragma once


#include "Event.h"
#include "input.h"

namespace nyl {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: Event(EventType::MouseMoved), m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }
		static EventType GetStaticEventType() { return EventType::MouseMoved; }


	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: Event(EventType::MouseScrolled), m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }
		static EventType GetStaticEventType() { return EventType::MouseScrolled; }

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(const MouseCode button, const float x, const float y)
			: Event(EventType::MouseButtonPressed), m_Button(button), m_X(x), m_Y(y) {}
		MouseCode GetMouseButton() const { return m_Button; }
		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }
		static EventType GetStaticEventType() { return EventType::MouseButtonPressed; }
	private:
		MouseCode m_Button;
		float m_X, m_Y;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button, const float x, const float y)
			: Event(EventType::MouseButtonReleased), m_Button(button), m_X(x), m_Y(y) {}
		MouseCode GetMouseButton() const { return m_Button; }
		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }
		static EventType GetStaticEventType() { return EventType::MouseButtonReleased; }
	private:
		MouseCode m_Button;
		float m_X, m_Y;
	};

}