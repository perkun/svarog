#ifndef EVENT_H_
#define EVENT_H_

#include <iostream>
#include <stdio.h>
#include <functional>

using namespace std;

enum class EventType {
	none = 0,
	key_pressed, key_released, mouse_button_pressed,
	mouse_button_released, cursor_moved
};


class Event
{
public:
	virtual EventType get_event_type() = 0;
	virtual void print_type() = 0;


};






class EventDispacher
{
public:
	EventDispacher(Event& e) : event(e) {};

	template<typename T>
	void dispatch(function<void(T&)> func)
	{
		if (event.get_event_type() == T::get_static_event_type())
		{
			func( *(T*)&event );
		}
	}
private:
	Event &event;
};

#endif /* EVENT_H_ */
