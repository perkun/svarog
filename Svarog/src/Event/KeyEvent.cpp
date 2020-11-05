#include "KeyEvent.h"

KeyPressedEvent::KeyPressedEvent(int key_code, int repeat_count)
	: key_code(key_code), repeat_count(repeat_count)
{
}


EventType KeyPressedEvent::get_event_type()
{
	return get_static_event_type();
}


EventType KeyPressedEvent::get_static_event_type()
{
	return EventType::key_pressed;
}


void KeyPressedEvent::print_type()
{
	cout << "KeyPressedEvent: " << key_code << ",  " << repeat_count << endl;
}

int KeyPressedEvent::get_key_code()
{
	return key_code;
}


int KeyPressedEvent::get_repeat_count()
{
	return repeat_count;
}








KeyReleasedEvent::KeyReleasedEvent(int key_code)
	: key_code(key_code)
{
}


EventType KeyReleasedEvent::get_event_type()
{
	return get_static_event_type();
}


EventType KeyReleasedEvent::get_static_event_type()
{
	return EventType::key_released;
}


void KeyReleasedEvent::print_type()
{
	cout << "KeyReleasedEvent: " << key_code << ",  " << endl;
}

int KeyReleasedEvent::get_key_code()
{
	return key_code;
}


