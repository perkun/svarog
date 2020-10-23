#ifndef SVAROG_H_
#define SVAROG_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <map>
#include <functional>

#include "MemmoryLedger.h"
#include "Mesh.h"
#include "Transform.h"
#include "SceneNode.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Camera.h"


using namespace std;

class Svarog
{
public:
	Svarog(int, int, string, bool fullscreen = false, bool visible = true);
	~Svarog();

	Mesh* create_mesh(string filename, int mode);
	void close_window(int);

	void on_event(Event&);
	void on_key_pressed_event(KeyPressedEvent&);
	void on_key_released_event(KeyReleasedEvent&);
	void on_mouse_button_pressed_event(MouseButtonPressedEvent&);
	void on_mouse_button_released_event(MouseButtonReleasedEvent&);
	void on_mouse_scrolled_event(MouseScrolledEvent&);
	void on_curosr_moved_event(MouseMovedEvent&);
	void on_window_resize_event(WindowResizeEvent&);

// 	void rendering_loop();

	SceneNode *scene_graph_root = NULL;
	SceneNode *current_node = NULL;
	Camera *current_camera;

	map<int, function<void(Svarog*)> > key_pressed_map;
	map<int, function<void(Svarog*)> > key_released_map;
	map<int, function<void(Svarog*)> > mouse_button_pressed_map;
	map<int, function<void(Svarog*)> > mouse_button_released_map;

	function<void(Svarog*, vec2 cursor_shift)> mouse_cursor_action = NULL;
	function<void(Svarog*, vec2 offset)> mouse_scrolled_action = NULL;

	Window *window;

	vec2 cursor_pos;

private:

	MemmoryLedger<Mesh*> mesh_ledger;

};

#endif /* SVAROG_H_ */
