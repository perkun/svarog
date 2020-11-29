#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class CameraController : public ScriptableEntity
{
public:
    ~CameraController() { }

protected:
    vec2 cursor_pos;

    virtual void on_create() override
    {
    }
    virtual void on_destroy() override
    {
    }

    virtual void on_update(double time_delta) override
    {
//         if (Input::is_imgui_window_hovered())
//             return;

        Transform &ot = get_component<Transform>();

        if (Input::is_key_pressed(GLFW_KEY_W))
            ot.pan_forwards(time_delta);
        if (Input::is_key_pressed(GLFW_KEY_S))
            ot.pan_backwards(time_delta);
        if (Input::is_key_pressed(GLFW_KEY_A))
            ot.pan_left(time_delta);
        if (Input::is_key_pressed(GLFW_KEY_D))
            ot.pan_right(time_delta);
    }

    virtual void on_event(Event &e) override
    {
        EventDispacher dispatcher(e);

        dispatcher.dispatch<MouseMovedEvent>(bind(
            &CameraController::on_cursor_moved_event, this, placeholders::_1));

        dispatcher.dispatch<MouseScrolledEvent>(bind(
            &CameraController::on_mouse_scrolled_event, this, placeholders::_1));
    }

    void on_mouse_scrolled_event(MouseScrolledEvent &e)
    {
		Transform &ot = get_component<Transform>();
        ot.position += ot.front * (float)(e.get_offset().y / 5. * ot.speed);
    }

    void on_cursor_moved_event(MouseMovedEvent &e)
    {
//         if (Input::is_imgui_window_hovered())
//             return;

		vec2 cursor_shift = e.get_cursor_pos() - cursor_pos;
        if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_3))
            get_component<Transform>().rotate_about_target(cursor_shift);
		cursor_pos= e.get_cursor_pos();
    }

};

#endif /* CAMERACONTROLLER_H_ */
