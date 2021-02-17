#ifndef TIMEPANEL_H_
#define TIMEPANEL_H_

#include "Scene.h"
#include "Utils/Time.h"

using namespace std;

class TimePanel
{
public:
	TimePanel() = default;
	TimePanel(Scene*);
	~TimePanel();

	void on_imgui_render();

	double julian_day;
	Time::CalendarDate date;

private:
	Scene *scene;
	void draw_time_selection();
	void set_rotations(double julian_day);
	void set_rotations(Time::CalendarDate date);

};

#endif /* TIMEPANEL_H_ */
