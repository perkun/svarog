#ifndef WINDOWEVENT_H_
#define WINDOWEVENT_H_

#include "Event.h"


using namespace std;
using namespace glm;

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(int, int);
	~WindowResizeEvent();

	ivec2 get_size();
	void print_type();
	static EventType get_static_event_type();
	EventType get_event_type();

private:
	int width, height;
};

#endif /* WINDOWEVENT_H_ */
