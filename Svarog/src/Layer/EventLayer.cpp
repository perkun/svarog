#include "EventLayer.h"


EventLayer::EventLayer() : Layer("EventLayer")
{
    // FRAMEBUFFER
    FramebufferSpec fb_spec;
    fb_spec.width = Application::get_window()->width;
    fb_spec.height = Application::get_window()->height;
	fb_spec.flags |= COLOR_ATTACHMENT;

    framebuffer = new Framebuffer(fb_spec);
	scene = new Scene();
}


EventLayer::~EventLayer()
{
	delete scene;
	delete framebuffer;
}


void EventLayer::on_attach()
{

}

void EventLayer::on_detach()
{
}



void EventLayer::on_update(double ts)
{

}


void EventLayer::on_imgui_render()
{
}

// EVENTS
void EventLayer::on_event(Event &event)
{
    EventDispacher dispatcher(event);

    dispatcher.dispatch<WindowResizeEvent>(
        bind(&EventLayer::on_window_resize_event, this, placeholders::_1));

    dispatcher.dispatch<KeyPressedEvent>(
        bind(&EventLayer::on_key_pressed_event, this, placeholders::_1));
//
    dispatcher.dispatch<KeyReleasedEvent>(
        bind(&EventLayer::on_key_released_event, this, placeholders::_1));
//
//
	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        {
            dispatcher.dispatch<MouseButtonPressedEvent>(
                bind(&EventLayer::on_mouse_button_pressed_event, this,
                     placeholders::_1));
//
            dispatcher.dispatch<MouseButtonReleasedEvent>(
                bind(&EventLayer::on_mouse_button_released_event, this,
                     placeholders::_1));
//
            dispatcher.dispatch<MouseMovedEvent>(bind(
                &EventLayer::on_curosr_moved_event, this, placeholders::_1));
//
            dispatcher.dispatch<MouseScrolledEvent>(bind(
                &EventLayer::on_mouse_scrolled_event, this, placeholders::_1));
        }
}


void EventLayer::on_window_resize_event(WindowResizeEvent &event)
{
    ivec2 size = event.get_size();
    scene->on_resize(size.x, size.y);
	framebuffer->resize(size.x, size.y);
}


void EventLayer::on_mouse_button_released_event(MouseButtonReleasedEvent &event)
{
    int button_code = event.get_button_code();

    if (mouse_button_released_map.find(button_code) !=
        mouse_button_released_map.end())
        mouse_button_released_map[button_code](this);
}


void EventLayer::on_mouse_button_pressed_event(MouseButtonPressedEvent &event)
{
    int button_code = event.get_button_code();

    if (mouse_button_pressed_map.find(button_code) !=
        mouse_button_pressed_map.end())
        mouse_button_pressed_map[button_code](this);
}



void EventLayer::on_key_pressed_event(KeyPressedEvent &event)
{
	int key_code = event.get_key_code();
	int repeat_count = event.get_repeat_count();

    if (key_pressed_map.find(key_code) != key_pressed_map.end())
		key_pressed_map[key_code](this);
}

void EventLayer::on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();
    if (key_released_map.find(key_code) != key_released_map.end())
		key_released_map[key_code](this);
}


void EventLayer::on_mouse_scrolled_event(MouseScrolledEvent &event)
{
	vec2 offset = event.get_offset();
	if (mouse_scrolled_action != NULL)
		mouse_scrolled_action(this, offset);
}

void EventLayer::on_curosr_moved_event(MouseMovedEvent &event)
{
	vec2 cursor_shift;
	vec2 old_cursor_pos = Application::get_cursor_pos();
	vec2 new_cursor_pos = event.get_cursor_pos();

	Application::set_cursor_pos(new_cursor_pos);

	cursor_shift = new_cursor_pos - old_cursor_pos;

	if (mouse_cursor_action != NULL)
		mouse_cursor_action(this, cursor_shift);
}
