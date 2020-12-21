#ifndef MODELCONTROLLER_H_
#define MODELCONTROLLER_H_

#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"

using namespace std;

class ModelController : public ScriptableEntity

{
public:
    ModelController() {}

    virtual void on_create() override
    {
    }

    virtual void on_destroy() override
    {
    }

    virtual void on_update(double time_delta) override
    {

	}

    virtual void on_event(Event &e) override
    {
        EventDispacher dispatcher(e);

        dispatcher.dispatch<MouseMovedEvent>(bind(
            &ModelController::on_cursor_moved_event, this, placeholders::_1));

//         dispatcher.dispatch<MouseScrolledEvent>(bind(
//             &ModelController::on_mouse_scrolled_event, this, placeholders::_1));
    }

    void on_mouse_scrolled_event(MouseScrolledEvent &e)
    {
		Transform &t = get_component<Transform>();
        t.scale += 0.1 * e.get_offset().y;
    }

    void on_cursor_moved_event(MouseMovedEvent &e)
    {
//         if (Input::is_imgui_window_hovered())
//             return;


		vec2 cursor_shift = e.get_cursor_pos() - cursor_pos;
        if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_1)
			&& Input::is_key_pressed(GLFW_KEY_LEFT_CONTROL))
		{
			Transform &t = get_component<Transform>();
// 			t.alpha += t.rotation_speed * (cursor_shift.x);
//             t.beta += t.rotation_speed * (cursor_shift.y);
// 			t.gamma += cursor_shift.x * t.rotation_speed;
// 			t.beta +=  cursor_shift.y * t.rotation_speed;
			t.rotation.x += cursor_shift.y * t.rotation_speed;
			t.rotation.y += cursor_shift.x * t.rotation_speed;
		}

		cursor_pos= e.get_cursor_pos();
    }

    vec2 cursor_pos;
};

#endif /* MODELCONTROLLER_H_ */
