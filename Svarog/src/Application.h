#ifndef APPLICATION_H_
#define APPLICATION_H_


#include "Core.h"
#include "Renderer.h"
#include "Event/Event.h"
#include "LayerStack.h"
#include "Window.h"


using namespace std;
using namespace glm;

class Layer;
class ImGuiLayer;

enum class GlfwEventMethod
{
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

void push_layer(Layer *);
void push_overlay(Layer *);
void pop_layer(Layer *);
void pop_overlay(Layer *);

void on_event(Event &);
void on_window_resize_event(WindowResizeEvent&);
void on_key_released_event(KeyReleasedEvent&);
void on_curosr_moved_event(MouseMovedEvent&);
void on_mouse_scrolled_event(MouseScrolledEvent&);

void run(GlfwEventMethod);
void stop();

vec2 get_cursor_pos();
void set_cursor_pos(vec2);
vec2 get_cursor_shift();

void set_mouse_scroll_offset(float);
float get_mouse_scroll_offset();

void enable_blend();
void disable_blend();

vec4 get_bg_color();
void set_bg_color(vec4);

Window* get_window();
ImGuiLayer* get_imgui_layer();
} // namespace Application

#endif
