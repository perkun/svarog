#include "svpch.h"
#include "WindowEvent.h"

WindowResizeEvent::WindowResizeEvent(int width, int height)
    : width(width), height(height)
{
}

WindowResizeEvent::~WindowResizeEvent()
{
}

void WindowResizeEvent::print_type()
{
	cout << "WindowResizeEvent: " << width << ", " << height << endl;
}

EventType WindowResizeEvent::get_event_type()
{
	return get_static_event_type();
}


EventType WindowResizeEvent::get_static_event_type()
{
	return EventType::window_resized;
}

ivec2 WindowResizeEvent::get_size()
{
	return ivec2(width, height);
}
