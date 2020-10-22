#include "MouseEvent.h"

MouseButtonPressEvent::MouseButtonPressEvent(int button_code)
	: button_code(button_code)
{
}

void MouseButtonPressEvent::print_type()
{
	cout << "MouseButtonPressEvent: " << button_code << endl;
}

int MouseButtonPressEvent::get_button_code()
{
	return button_code;
}

EventType MouseButtonPressEvent::get_event_type()
{
	return get_static_event_type();
}


EventType MouseButtonPressEvent::get_static_event_type()
{
	return EventType::mouse_button_pressed;
}








MouseMovedEvent::MouseMovedEvent(vec2 cursor_pos) : cursor_pos(cursor_pos)
{
}

vec2 MouseMovedEvent::get_cursor_pos()
{
	return cursor_pos;
}

void MouseMovedEvent::print_type()
{
	cout << "MouseMovedEvent: " << cursor_pos.x << ", " << cursor_pos.y << endl;
}


EventType MouseMovedEvent::get_event_type()
{
	return get_static_event_type();
}


EventType MouseMovedEvent::get_static_event_type()
{
	return EventType::cursor_moved;
}
