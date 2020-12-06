#ifndef KEYEVENT_H_
#define KEYEVENT_H_

#include "Event.h"

using namespace std;

class KeyPressedEvent: public Event
{
public:
	KeyPressedEvent(int, int);
	int get_key_code();
	int get_repeat_count();

	void print_type();
	static EventType get_static_event_type();
	EventType get_event_type();

private:
	int key_code;
	int repeat_count;

};


class KeyReleasedEvent: public Event
{
public:
	KeyReleasedEvent(int);
	int get_key_code();

	void print_type();
	static EventType get_static_event_type();
	EventType get_event_type();

private:
	int key_code;

};


#endif
