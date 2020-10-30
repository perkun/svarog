#ifndef WINDOW_H_
#define WINDOW_H_

#include <iostream>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

using namespace std;

class Window
{
public:
	Window(int, int, string, bool, bool);
	~Window();

	void show();
	void hide();
// 	void create(int, int, string title, bool visible);
	void destroy();
	void close();

	void set_event_callback_fn(const function<void(Event&)> &func);

	GLFWwindow* winptr;
	int width, height;
	string title;

	function< void(Event&) > event_callback_fn;

};

#endif /* WINDOW_H_ */
