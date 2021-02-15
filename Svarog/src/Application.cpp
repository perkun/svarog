#include "svpch.h"
#include "Application.h"
#include "ImGuiLayer.h"
#include "Renderer.h"

// #define BG_COLOR 41 / 256., 46 / 256., 48 / 256., 1.0

namespace Application
{
// extern variables definitions
map<string, shared_ptr<Shader> > shaders;

// private variables
Window *window = NULL;
LayerStack layer_stack;
ImGuiLayer *imgui_layer;
vec2 cursor_pos, cursor_shift;
float mouse_scroll_offset;

vec4 bg_color(41 / 256., 46 / 256., 48 / 256., 1.0);

bool should_run = true;
////


vec4 get_bg_color()
{
	return bg_color;
}

void set_bg_color(vec4 new_color)
{
	bg_color = new_color;
	Renderer::clear(new_color);
}


float get_mouse_scroll_offset()
{
	return mouse_scroll_offset;
}

void set_mouse_scroll_offset(float new_mso)
{
	mouse_scroll_offset = new_mso;
}


vec2 get_cursor_pos()
{
	return cursor_pos;
}

vec2 get_cursor_shift()
{
	return cursor_shift;
}

void set_cursor_pos(vec2 cp)
{
	cursor_pos = cp;
}

ImGuiLayer* get_imgui_layer()
{
	return imgui_layer;
}

Window *get_window()
{
    return window;
}

void init(int width, int height, string w_title, bool fullscreen, bool visible)
{
    window = new Window(width, height, w_title, fullscreen, visible);
    window->set_event_callback_fn(bind(&on_event, placeholders::_1));
    //       bind(&on_event, this, placeholders::_1));

    // start GLEW extension handler
    GLenum err = glewInit();
    if (err != GLEW_OK)
        cout << "glew init failed" << endl;


    //     scene_graph_root = new SceneNode();

    cursor_pos = vec2(width / 2.0, height / 2.0);

	imgui_layer = new ImGuiLayer;
	push_overlay(imgui_layer);

	Renderer::init();
	Renderer::clear(bg_color);

	// Logging
	Log::init();

	CORE_TRACE("Application initialized");

}

// ~Application()
void destroy()
{
	spdlog::shutdown();

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

void on_event(Event &event)
{
    EventDispacher dispatcher(event);

    // 	dispatcher.dispatch<KeyPressedEvent>(
    // 		bind(&on_key_pressed_event, this, placeholders::_1));
    //
//     dispatcher.dispatch<KeyReleasedEvent>(
//         bind(&on_key_released_event, placeholders::_1));
    // 		bind(&on_key_released_event, this, placeholders::_1));
    //
    // 	dispatcher.dispatch<MouseButtonPressedEvent>(
    // 		bind(&on_mouse_button_pressed_event, this, placeholders::_1));
    //
    // 	dispatcher.dispatch<MouseButtonReleasedEvent>(
    // 		bind(&on_mouse_button_released_event, this, placeholders::_1));
    //
//     dispatcher.dispatch<MouseMovedEvent>(
//     	bind(&on_curosr_moved_event, placeholders::_1));


// 	dispatcher.dispatch<MouseScrolledEvent>(
// 		bind(&on_mouse_scrolled_event, placeholders::_1));

    dispatcher.dispatch<WindowResizeEvent>(
        bind(&on_window_resize_event, placeholders::_1));
    // 		bind(&on_window_resize_event, this, placeholders::_1));

	// propagate events to layers
    for (auto it = layer_stack.rbegin(); it != layer_stack.rend(); ++it)
    {
        if (event.handled)
            break;

        (*it)->on_event(event);
    }
}

void on_window_resize_event(WindowResizeEvent &event)
{
    const ivec2 &size = event.get_size();
	window->width = size.x;
	window->height = size.y;
    Renderer::set_viewport(0., 0., size.x, size.y);
}

void on_mouse_scrolled_event(MouseScrolledEvent& event)
{
// 	cout << event.get_offset().y << endl;
	mouse_scroll_offset = event.get_offset().y;
// 	if (mouse_scrolled_action != NULL)
// 		mouse_scrolled_action(this, event.get_offset());
}

void on_curosr_moved_event(MouseMovedEvent& event)
{
// 	cout << "on_curosr_moved_event    ";
// 	event.print_type();
	vec2 old_cursor_pos = cursor_pos;

	cursor_pos = event.get_cursor_pos();
	cursor_shift = cursor_pos - old_cursor_pos;

// 	if (mouse_cursor_action != NULL)
// 		mouse_cursor_action(this, cursor_shift);
//
}
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
	{
//         glfwSetWindowShouldClose(window->winptr, GLFW_TRUE);
         stop();

	}
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

void stop()
{
	should_run = false;
}

void run(GlfwEventMethod glfw_event_method)
{
    double time = glfwGetTime();
    double previous_time;
    double time_delta;

//     while (!glfwWindowShouldClose(window->winptr))
    while (should_run)
    {

        previous_time = time;
        time = glfwGetTime();
        time_delta = time - previous_time;

        // RENDER STUFF
        Renderer::clear();


        for (Layer *layer : layer_stack)
        {
            layer->on_update(time_delta);
        }

		imgui_layer->begin();
		{
			for (Layer* layer : layer_stack)
				layer->on_imgui_render();
		}
		imgui_layer->end();


        glfwSwapBuffers(window->winptr);

        if (glfw_event_method == GlfwEventMethod::POLL)
            glfwPollEvents();
        else if (glfw_event_method == GlfwEventMethod::WAIT)
            glfwWaitEvents();
    }

	CORE_TRACE("ENDING RENDERING LOOP");
}


} // namespace Application

