#include "Svarog.h"

Svarog::Svarog(int width, int height, string w_title, bool visible)
{
	window = new Window(width, height, w_title, visible);
	window->set_event_callback_fn(bind(&Svarog::on_event, this, placeholders::_1));

	// start GLEW extension handler
	GLenum err = glewInit();
	if (err != GLEW_OK)
		cout << "glew init failed" << endl;


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace (GL_CCW);

	scene_graph_root = new SceneNode();

	cursor_pos = vec2(width/2.0, height/2.0);
}


Svarog::~Svarog()
{
	mesh_ledger.delete_all();

	delete scene_graph_root;
	delete window;
}

void Svarog::on_event(Event& event)
{
	EventDispacher dispatcher(event);

	dispatcher.dispatch<KeyPressedEvent>(
		bind(&Svarog::on_key_pressed_event, this, placeholders::_1));

	dispatcher.dispatch<KeyReleasedEvent>(
		bind(&Svarog::on_key_released_event, this, placeholders::_1));

	dispatcher.dispatch<MouseButtonPressEvent>(
		bind(&Svarog::on_mouse_button_press_event, this, placeholders::_1));

	dispatcher.dispatch<MouseMovedEvent>(
		bind(&Svarog::on_curosr_moved_event, this, placeholders::_1));

}

void Svarog::on_curosr_moved_event(MouseMovedEvent& event)
{
// 	cout << "on_curosr_moved_event    ";
// 	event.print_type();
	vec2 cursor_shift, old_cursor_pos = cursor_pos;

	cursor_pos = event.get_cursor_pos();
	cursor_shift = cursor_pos - old_cursor_pos;

	mouse_cursor_action(current_node, current_camera, cursor_shift);

}


void Svarog::on_mouse_button_press_event(MouseButtonPressEvent& event)
{
	int button_code = event.get_button_code();
// 	cout << "on_mouse_button_press_event    ";
// 	event.print_type();

}


void Svarog::on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();

	if (key_released_map.find(key_code) != key_released_map.end())
		key_released_map[key_code](current_node, current_camera);


// 	event.print_type();
	return;
}

void Svarog::on_key_pressed_event(KeyPressedEvent &event)
{
	int key_code = event.get_key_code();
	int repeat_count = event.get_repeat_count();
// 	cout << "on_key_pressed_event    ";
// 	event.print_type();


	if (key_code == GLFW_KEY_Q)
		glfwSetWindowShouldClose(window->winptr, GLFW_TRUE);

	if (key_pressed_map.find(key_code) != key_pressed_map.end())
		key_pressed_map[key_code](current_node, current_camera);

}



Mesh* Svarog::create_mesh(string filename, int mode)
{
	Mesh *m = new Mesh(filename, mode);
	mesh_ledger.add(m);
	return m;
}

// void Svarog::rendering_loop() {
//
// 	while (!glfwWindowShouldClose(svarog.window->winptr)) {
// 		// RENDER STUFF
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
// 		svarog.scene_graph_root->update_depth_first();
// 		svarog.scene_graph_root->draw_depth_first(camera.get_view(),
// 				camera.get_perspective());
// 		glfwSwapBuffers(svarog.window->winptr);
// 		glfwPollEvents();
// 	}
// }
