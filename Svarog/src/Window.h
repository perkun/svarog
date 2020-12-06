#ifndef WINDOW_H_
#define WINDOW_H_

#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

using namespace std;

class GLFWwindow;

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

	void set_cursor_hidden();
	void set_cursor_normal();
	void set_cursor_disabled();


	GLFWwindow* winptr;
	int width, height;
	string title;

	function< void(Event&) > event_callback_fn;
private:
	void set_cursor_mode(unsigned int);

};

#endif /* WINDOW_H_ */
