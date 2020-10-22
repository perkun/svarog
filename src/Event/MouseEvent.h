#ifndef MOUSEEVENT_H_
#define MOUSEEVENT_H_

#include <iostream>
#include <stdio.h>
#include <functional>
#include <glm/glm.hpp>
#include "Event.h"

using namespace std;
using namespace glm;

class MouseButtonPressEvent: public Event
{
public:
	MouseButtonPressEvent(int);
	int get_button_code();

	void print_type();
	static EventType get_static_event_type();
	EventType get_event_type();

private:
	int button_code;

};


class MouseMovedEvent: public Event
{
public:
	MouseMovedEvent(vec2);
	vec2 get_cursor_pos();

	void print_type();
	static EventType get_static_event_type();
	EventType get_event_type();
private:
	vec2 cursor_pos;


};

#endif
