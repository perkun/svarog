#include "Application.h"

// private variables
namespace Application
{
	Window *window = NULL;
	vec2 cursor_pos;
	LayerStack layer_stack;


Window* get_window()
{
	return window;
}

void init(int width, int height, string w_title, bool fullscreen,
                       bool visible)
{
  window = new Window(width, height, w_title, fullscreen, visible);
  window->set_event_callback_fn(
      bind(&on_event, placeholders::_1));
//       bind(&on_event, this, placeholders::_1));

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

// ~Application()
void destroy()
{
	if (window != NULL)
		delete window;
}

void push_layer(Layer *layer)
{
	layer_stack.push_layer(layer);
	layer->on_attach();
}


void push_overlay(Layer *layer)
{
	layer_stack.push_overlay(layer);
	layer->on_attach();
}

void pop_layer(Layer *layer)
{
	layer_stack.pop_layer(layer);
	layer->on_detach();
}


void pop_overlay(Layer *layer)
{
	layer_stack.pop_overlay(layer);
	layer->on_detach();
}


void on_event(Event& event)
{
	EventDispacher dispatcher(event);

// 	dispatcher.dispatch<KeyPressedEvent>(
// 		bind(&on_key_pressed_event, this, placeholders::_1));
//
	dispatcher.dispatch<KeyReleasedEvent>(
		bind(&on_key_released_event, placeholders::_1));
// 		bind(&on_key_released_event, this, placeholders::_1));
//
// 	dispatcher.dispatch<MouseButtonPressedEvent>(
// 		bind(&on_mouse_button_pressed_event, this, placeholders::_1));
//
// 	dispatcher.dispatch<MouseButtonReleasedEvent>(
// 		bind(&on_mouse_button_released_event, this, placeholders::_1));
//
// 	dispatcher.dispatch<MouseMovedEvent>(
// 		bind(&on_curosr_moved_event, this, placeholders::_1));
//
// 	dispatcher.dispatch<MouseScrolledEvent>(
// 		bind(&on_mouse_scrolled_event, this, placeholders::_1));

	dispatcher.dispatch<WindowResizeEvent>(
		bind(&on_window_resize_event, placeholders::_1));
// 		bind(&on_window_resize_event, this, placeholders::_1));

	for (auto it = layer_stack.rbegin(); it != layer_stack.rend(); ++it)
	{
		if (event.handled)
			break;

		(*it)->on_event(event);
	}
}


void on_window_resize_event(WindowResizeEvent &event)
{
	ivec2 size = event.get_size();
	glViewport(0., 0., size.x, size.y);

}


// void on_mouse_scrolled_event(MouseScrolledEvent& event)
// {
// // 	cout << event.get_offset().y << endl;
// 	if (mouse_scrolled_action != NULL)
// 		mouse_scrolled_action(this, event.get_offset());
// }
//
// void on_curosr_moved_event(MouseMovedEvent& event)
// {
// // 	cout << "on_curosr_moved_event    ";
// // 	event.print_type();
// 	vec2 cursor_shift, old_cursor_pos = cursor_pos;
//
// 	cursor_pos = event.get_cursor_pos();
// 	cursor_shift = cursor_pos - old_cursor_pos;
//
// 	if (mouse_cursor_action != NULL)
// 		mouse_cursor_action(this, cursor_shift);
//
// }
//
//
// void on_mouse_button_released_event(MouseButtonReleasedEvent &event)
// {
//     int button_code = event.get_button_code();
//
//     if (mouse_button_released_map.find(button_code) !=
//         mouse_button_released_map.end())
//         mouse_button_released_map[button_code](this);
// }
//
//
// void on_mouse_button_pressed_event(MouseButtonPressedEvent &event)
// {
//     int button_code = event.get_button_code();
//
//     if (mouse_button_pressed_map.find(button_code) !=
//         mouse_button_pressed_map.end())
//         mouse_button_pressed_map[button_code](this);
//
//     // 	cout << "on_mouse_button_pressed_event    ";
//     // 	event.print_type();
// }
//
void on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();

	if (key_code == GLFW_KEY_Q)
		glfwSetWindowShouldClose(window->winptr, GLFW_TRUE);

}
//
// void on_key_pressed_event(KeyPressedEvent &event)
// {
// 	int key_code = event.get_key_code();
// 	int repeat_count = event.get_repeat_count();
// // 	cout << "on_key_pressed_event    ";
// // 	event.print_type();
//
//
// 	if (key_code == GLFW_KEY_Q)
// 		glfwSetWindowShouldClose(window->winptr, GLFW_TRUE);
// 	else if (key_pressed_map.find(key_code) != key_pressed_map.end())
// 		key_pressed_map[key_code](this);
//
// }


void rendering_loop(GlfwEventMethod glfw_event_method)
{
	Renderer renderer;

    double time = glfwGetTime();
    double previous_time;
    double time_delta;

// 	if (active_scene == NULL)
// 	{
// 		cout << "No active Scene set, not rendering" << endl;
// 		return;
// 	}
//
// 	if (active_scene->get_active_camera() == NULL)
// 	{
// 		cout << "No active Camera set, not rendering" << endl;
// 		return;
// 	}

    while (!glfwWindowShouldClose(window->winptr))
    {

		previous_time = time;
		time = glfwGetTime();
		time_delta = time - previous_time;

        // RENDER STUFF
		renderer.clear(BG_COLOR);

// 		scene.draw_depth_first(scene.root_entity, &camera);

		for (Layer *layer: layer_stack)
		{
			layer->on_update(time_delta);
		}

        glfwSwapBuffers(window->winptr);

		if (glfw_event_method == GlfwEventMethod::POLL)
			glfwPollEvents();
		else if (glfw_event_method == GlfwEventMethod::WAIT)
			glfwWaitEvents();

    }
}

}

// Application* get()
// {
// 	return this;
// }


