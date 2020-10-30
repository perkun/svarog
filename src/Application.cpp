#include "Application.h"

Application::Application(int width, int height, string w_title, bool fullscreen,
               bool visible)
{
    window = new Window(width, height, w_title, fullscreen, visible);
    window->set_event_callback_fn(
        bind(&Application::on_event, this, placeholders::_1));

    // start GLEW extension handler
    GLenum err = glewInit();
    if (err != GLEW_OK)
        cout << "glew init failed" << endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     scene_graph_root = new SceneNode();

    cursor_pos = vec2(width / 2.0, height / 2.0);
}


Application::~Application()
{
	delete window;
}


void Application::on_event(Event& event)
{
	EventDispacher dispatcher(event);

	dispatcher.dispatch<KeyPressedEvent>(
		bind(&Application::on_key_pressed_event, this, placeholders::_1));

	dispatcher.dispatch<KeyReleasedEvent>(
		bind(&Application::on_key_released_event, this, placeholders::_1));

	dispatcher.dispatch<MouseButtonPressedEvent>(
		bind(&Application::on_mouse_button_pressed_event, this, placeholders::_1));

	dispatcher.dispatch<MouseButtonReleasedEvent>(
		bind(&Application::on_mouse_button_released_event, this, placeholders::_1));

	dispatcher.dispatch<MouseMovedEvent>(
		bind(&Application::on_curosr_moved_event, this, placeholders::_1));

	dispatcher.dispatch<MouseScrolledEvent>(
		bind(&Application::on_mouse_scrolled_event, this, placeholders::_1));

	dispatcher.dispatch<WindowResizeEvent>(
		bind(&Application::on_window_resize_event, this, placeholders::_1));

}


void Application::on_window_resize_event(WindowResizeEvent &event)
{
	ivec2 size = event.get_size();
	glViewport(0., 0., size.x, size.y);

	active_scene->on_resize(size.x, size.y);
}


void Application::on_mouse_scrolled_event(MouseScrolledEvent& event)
{
// 	cout << event.get_offset().y << endl;
	if (mouse_scrolled_action != NULL)
		mouse_scrolled_action(this, event.get_offset());
}

void Application::on_curosr_moved_event(MouseMovedEvent& event)
{
// 	cout << "on_curosr_moved_event    ";
// 	event.print_type();
	vec2 cursor_shift, old_cursor_pos = cursor_pos;

	cursor_pos = event.get_cursor_pos();
	cursor_shift = cursor_pos - old_cursor_pos;

	if (mouse_cursor_action != NULL)
		mouse_cursor_action(this, cursor_shift);

}


void Application::on_mouse_button_released_event(MouseButtonReleasedEvent &event)
{
    int button_code = event.get_button_code();

    if (mouse_button_released_map.find(button_code) !=
        mouse_button_released_map.end())
        mouse_button_released_map[button_code](this);
}


void Application::on_mouse_button_pressed_event(MouseButtonPressedEvent &event)
{
    int button_code = event.get_button_code();

    if (mouse_button_pressed_map.find(button_code) !=
        mouse_button_pressed_map.end())
        mouse_button_pressed_map[button_code](this);

    // 	cout << "on_mouse_button_pressed_event    ";
    // 	event.print_type();
}

void Application::on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();

	if (key_released_map.find(key_code) != key_released_map.end())
		key_released_map[key_code](this);


// 	event.print_type();
	return;
}

void Application::on_key_pressed_event(KeyPressedEvent &event)
{
	int key_code = event.get_key_code();
	int repeat_count = event.get_repeat_count();
// 	cout << "on_key_pressed_event    ";
// 	event.print_type();


	if (key_code == GLFW_KEY_Q)
		glfwSetWindowShouldClose(window->winptr, GLFW_TRUE);
	else if (key_pressed_map.find(key_code) != key_pressed_map.end())
		key_pressed_map[key_code](this);

}


void Application::rendering_loop(GlfwEventMethod glfw_event_method)
{
	Renderer renderer;

    double time = glfwGetTime();
    double previous_time;
    double time_delta;

	if (active_scene->get_active_camera() == NULL)
	{
		cout << "No active camera set, not rendering" << endl;
		return;
	}

    while (!glfwWindowShouldClose(window->winptr))
    {

		previous_time = time;
		time = glfwGetTime();
		time_delta = time - previous_time;

        // RENDER STUFF
		renderer.clear(BG_COLOR);

// 		scene.draw_depth_first(scene.root_entity, &camera);
		active_scene->draw_root();

        glfwSwapBuffers(window->winptr);

		if (glfw_event_method == GlfwEventMethod::POLL)
			glfwPollEvents();
		else if (glfw_event_method == GlfwEventMethod::WAIT)
			glfwWaitEvents();

		active_scene->get_active_camera()->move(time_delta);
    }


}
