#pragma once


#include "Event.h"
#include "input.h"

namespace nyl {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: Event(MouseMoved), m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: Event(MouseScrolled), m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

	private:
		float m_XOffset, m_YOffset;
	};

    class MouseButtonPressedEvent : public Event
    {
    public:
        MouseButtonEvent(const MouseCode button) : Event(MouseButtonPressed), m_Button(button) {}
        MouseCode GetMouseButton() const { return m_Button; }
    private:
        MouseCode m_Button;
    };

    class MouseButtonReleasedEvent : public Event
    {
    public:
        MouseButtonReleasedEvent(const MouseCode button) : Event(MouseButtonReleased), m_Button(button) {}
        MouseCode GetMouseButton() const { return m_Button; }
    private:
        MouseCode m_Button;
    };

}
