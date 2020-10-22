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


using namespace std;

class Svarog
{
public:
	Svarog(int, int, string, bool visible = true);
	~Svarog();

	Mesh* create_mesh(string filename, int mode);
	void close_window(int);

	void on_event(Event&);
	void on_key_pressed_event(KeyPressedEvent&);
	void on_key_released_event(KeyReleasedEvent&);
	void on_mouse_button_press_event(MouseButtonPressEvent&);
	void on_curosr_moved_event(MouseMovedEvent& event);

	SceneNode *scene_graph_root = NULL;
	SceneNode *current_node = NULL;
	// Camera *current_camera;

	map<int, function<void(SceneNode*)> > key_pressed_map;

	Window *window;

	map<int, std::function<void(int)> > key_map;

private:

	MemmoryLedger<Mesh*> mesh_ledger;

};

#endif /* SVAROG_H_ */
