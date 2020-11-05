#include "Window.h"

Window::Window(int w, int h, string title, bool fullscreen, bool visible)
{
	if (!glfwInit())
	{
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		exit(0);
	}
	width = w;
	height = h;
	this->title = title;

	// create window
	if (visible)
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	else
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	if (fullscreen)
		winptr = glfwCreateWindow(w, h, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else
		winptr = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(winptr);

	// user pointer
	glfwSetWindowUserPointer(winptr, this);

	// invisible cursor
// 	glfwSetInputMode(winptr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	// set callbacks
	glfwSetKeyCallback(winptr,
		[](GLFWwindow *win, int key, int scancode, int action, int mods)
		{
			Window *window = (Window*)glfwGetWindowUserPointer(win);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					window->event_callback_fn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					window->event_callback_fn(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					window->event_callback_fn(event);
					break;
				}
			}
		}
	);

	glfwSetMouseButtonCallback(winptr,
		[](GLFWwindow *win, int button, int action, int mods)
		{
			Window *window = (Window*)glfwGetWindowUserPointer(win);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					window->event_callback_fn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					window->event_callback_fn(event);
					break;
				}
			}


		}
	);

	glfwSetCursorPosCallback(winptr,
		[](GLFWwindow *win, double xpos, double ypos)
		{
			Window *window = (Window*)glfwGetWindowUserPointer(win);
			MouseMovedEvent event(vec2(xpos, ypos));
			window->event_callback_fn(event);
		}
	);

	glfwSetScrollCallback(winptr,
		[](GLFWwindow *win, double xoffset, double yoffset)
		{
			Window *window = (Window*)glfwGetWindowUserPointer(win);
			MouseScrolledEvent event(vec2(xoffset, yoffset));
			window->event_callback_fn(event);
		}
	);


	glfwSetWindowSizeCallback(winptr,
		[](GLFWwindow *win, int width, int height)
		{
			Window *window = (Window*)glfwGetWindowUserPointer(win);
			WindowResizeEvent event(width, height);
			window->event_callback_fn(event);
		}
	);


}



Window::~Window()
{
	destroy();
	glfwTerminate();
}


void Window::set_cursor_mode(unsigned int mode)
{
	glfwSetInputMode(winptr, GLFW_CURSOR, mode);
}


void Window::set_cursor_hidden()
{
	set_cursor_mode(GLFW_CURSOR_HIDDEN);
}

void Window::set_cursor_normal()
{

	set_cursor_mode(GLFW_CURSOR_NORMAL);
}

void Window::set_cursor_disabled()
{

	set_cursor_mode(GLFW_CURSOR_DISABLED);
}


// void Window::create(int w, int h, string title, bool visible)
// {
// 	if (visible)
// 		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
// 	else
// 		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
//
// 	// new window focus
// // 	glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
//
// 	winptr = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
// }

void Window::close()
{
	glfwSetWindowShouldClose(winptr, GLFW_TRUE);
}

void Window::destroy()
{
	glfwDestroyWindow(winptr);
}

void Window::show()
{
	glfwShowWindow(winptr);
}

void Window::hide()
{
	glfwHideWindow(winptr);
}

void Window::set_event_callback_fn(const function<void(Event&)> &func)
{
	event_callback_fn = func;
}
