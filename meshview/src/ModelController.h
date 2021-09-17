#ifndef MODELCONTROLLER_H_
#define MODELCONTROLLER_H_

#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"
#include "Camera.h"
#include "Math.h"


using namespace std;

class ModelController : public ScriptableEntity
{
public:
    ModelController()
    {
        accumulator = mat4(1.);
        x_axis = vec3(1., 0., 0.);
        y_axis = vec3(0., 1., 0.);
        z_axis = vec3(0., 0., 1.);
    }
    ~ModelController()
    {
    }

    vec2 last_cursor_pos;

    mat4 accumulator;
    float mouse_sensitivity = 1. / 225;

    void set_x_axis_view()
    {
        accumulator = glm::rotate(mat4(1.), (float)(-M_PI / 2.), z_axis);
		update_rotation();
    }
    void set_negative_x_axis_view()
    {
        accumulator = glm::rotate(mat4(1.), (float)(M_PI / 2.), z_axis);
		update_rotation();
    }

    void set_y_axis_view()
    {
        accumulator = glm::rotate(mat4(1.), (float)M_PI, z_axis);
		update_rotation();
    }

    void set_negative_y_axis_view()
    {
        accumulator = glm::rotate(mat4(1.), (float)(-M_PI / 2.), z_axis);
		update_rotation();
    }

    void set_z_axis_view()
    {
        accumulator = glm::rotate(mat4(1.), (float)(M_PI / 2.), x_axis);
		update_rotation();
    }

    void set_negative_z_axis_view()
    {
        accumulator = glm::rotate(mat4(1.), (float)(-M_PI / 2.), x_axis);
		update_rotation();
    }

protected:
    vec2 cursor_pos;
    vec3 x_axis, y_axis, z_axis;

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

        dispatcher.dispatch<KeyPressedEvent>(bind(
            &ModelController::on_key_pressed_event, this, placeholders::_1));

        dispatcher.dispatch<MouseMovedEvent>(bind(
            &ModelController::on_cursor_moved_event, this, placeholders::_1));
    }

    void on_key_pressed_event(KeyPressedEvent &event)
    {
        const int key_code = event.get_key_code();

        if (key_code == GLFW_KEY_Z)
        {
            if (Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
				set_negative_z_axis_view();
            else
				set_z_axis_view();
        }
        else if (key_code == GLFW_KEY_X)
        {
            if (Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
				set_negative_x_axis_view();
            else
				set_x_axis_view();
        }
        else if (key_code == GLFW_KEY_Y)
        {
            if (Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
				set_negative_y_axis_view();
            else
				set_y_axis_view();
        }
    }


    void on_cursor_moved_event(MouseMovedEvent &event)
    {
        vec2 cursor_shift = event.get_cursor_pos() - last_cursor_pos;

        if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_1))
        {
            float sf = 1920. * 0.47 / Application::get_window()->width;

            mat4 mrotz =
                glm::rotate(mat4(1.), mouse_sensitivity * cursor_shift.x * sf,
                            vec3(0, 0, 1));
            mat4 mrotx =
                glm::rotate(mat4(1.), mouse_sensitivity * cursor_shift.y * sf,
                            vec3(1, 0, 0));

            accumulator = mrotx * mrotz * accumulator;
            update_rotation();
        }

        last_cursor_pos = event.get_cursor_pos();
    }

    void update_rotation()
    {
        Transform &t = get_component<Transform>();
        vec3 trans, rot, scale;
        Math::decompose_transform(accumulator, trans, rot, scale);
        t.rotation = rot;
    }
};

#endif
