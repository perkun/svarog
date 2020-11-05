#include "MouseEvent.h"

MouseButtonPressedEvent::MouseButtonPressedEvent(int button_code)
	: button_code(button_code)
{
}

void MouseButtonPressedEvent::print_type()
{
	cout << "MouseButtonPressEvent: " << button_code << endl;
}

int MouseButtonPressedEvent::get_button_code()
{
	return button_code;
}

EventType MouseButtonPressedEvent::get_event_type()
{
	return get_static_event_type();
}


EventType MouseButtonPressedEvent::get_static_event_type()
{
	return EventType::mouse_button_pressed;
}


MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button_code)
	: button_code(button_code)
{
}

void MouseButtonReleasedEvent::print_type()
{
	cout << "MouseButtonReleasedEvent: " << button_code << endl;
}

int MouseButtonReleasedEvent::get_button_code()
{
	return button_code;
}

EventType MouseButtonReleasedEvent::get_event_type()
{
	return get_static_event_type();
}


EventType MouseButtonReleasedEvent::get_static_event_type()
{
	return EventType::mouse_button_released;
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





MouseScrolledEvent::MouseScrolledEvent(vec2 offset) : offset(offset)
{
}

vec2 MouseScrolledEvent::get_offset()
{
	return offset;
}

void MouseScrolledEvent::print_type()
{
	cout << "MouseMovedEvent: " << offset.x << ", " << offset.y << endl;
}


EventType MouseScrolledEvent::get_event_type()
{
	return get_static_event_type();
}


EventType MouseScrolledEvent::get_static_event_type()
{
	return EventType::mouse_scrolled;
}
