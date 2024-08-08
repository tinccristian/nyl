#pragma once

#include <list>
#include <functional>
#include <memory>
#include <utility>
#include "utils.h"
#include "core.h"

namespace nyl
{
	
    enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
 


    class NYL_API Event
    {
    public:
		Event(EventType eventType) : staticType(eventType) {};
        bool handled = false;
        virtual ~Event() = default;
        EventType GetNativeEventType() {return staticType;}

	private:
		EventType staticType;
    };


    class NYL_API EventDispatcher
	{
	public:
		static void AddEvent(std::unique_ptr<Event> event) { listEvent.push_back(std::move(event)); }
		
		template<typename T>
		static void DispatchEvent(std::function<bool(Event*)> func);
	private:
		static std::list<std::unique_ptr<Event>> listEvent;
	};


	template<typename T>
    void EventDispatcher::DispatchEvent(std::function<bool(Event*)> func)
	{
		for(auto event = listEvent.begin(); event != listEvent.end();)
		{
			if( T::GetStaticEventType() == ((*event)->GetNativeEventType()) )
			{
				if( !((*event)->handled) )
					(*event)->handled = func(static_cast<Event*>(event->get()));
				event = listEvent.erase(event); // Do I want to erase the event here?
			}
			else
			{
				++event;
			}
		}
	}

}