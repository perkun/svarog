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


// class Application
namespace Application
{
// public:
	void init(int, int, string, bool fullscreen = false, bool visible = true);
	void destroy();

	void close_window(int);

	void push_layer(Layer*);
	void push_overlay(Layer*);
	void pop_layer(Layer*);
	void pop_overlay(Layer*);

	void on_event(Event&);
	void on_window_resize_event(WindowResizeEvent&);
	void on_key_released_event(KeyReleasedEvent&);

	void rendering_loop(GlfwEventMethod);

	Window* get_window();

// 	Application* get();

// 	Window *window = NULL;
// 	vec2 cursor_pos;
//
// // private:
// 	LayerStack layer_stack;


}

#endif
