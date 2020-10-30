#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <map>
#include <functional>

#include "Mesh.h"
#include "Transform.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Camera.h"
#include "Scene/Scene.h"
#include "Renderer.h"
#include "LayerStack.h"

#define BG_COLOR 41/256., 46/256., 48/256., 1.0

using namespace std;


enum class GlfwEventMethod {
	POLL,
	WAIT
};


class Application
{
public:


	Application(int, int, string, bool fullscreen = false, bool visible = true);
	~Application();

	void close_window(int);

	void on_event(Event&);
	void on_key_pressed_event(KeyPressedEvent&);
	void on_key_released_event(KeyReleasedEvent&);
	void on_mouse_button_pressed_event(MouseButtonPressedEvent&);
	void on_mouse_button_released_event(MouseButtonReleasedEvent&);
	void on_mouse_scrolled_event(MouseScrolledEvent&);
	void on_curosr_moved_event(MouseMovedEvent&);
	void on_window_resize_event(WindowResizeEvent&);

	void rendering_loop(GlfwEventMethod);

	map<int, function<void(Application*)> > key_pressed_map;
	map<int, function<void(Application*)> > key_released_map;
	map<int, function<void(Application*)> > mouse_button_pressed_map;
	map<int, function<void(Application*)> > mouse_button_released_map;

	function<void(Application*, vec2 cursor_shift)> mouse_cursor_action = NULL;
	function<void(Application*, vec2 offset)> mouse_scrolled_action = NULL;

	Window *window;
// 	Scene *active_scene = NULL;

	vec2 cursor_pos;

	LayerStack layer_stack;
private:


};

#endif
