#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"

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
		ASSERT(has_component<CameraComponent>(),
			"CameraController ERROR: Entity does not have Camera Component");

		shared_ptr<Camera> cam = get_component<CameraComponent>().camera;

        if (Input::is_key_pressed(GLFW_KEY_W))
			cam->position += cam->front * (float)(time_delta * cam->speed);
        if (Input::is_key_pressed(GLFW_KEY_S))
			cam->position -= cam->front * (float)(time_delta * cam->speed);
        if (Input::is_key_pressed(GLFW_KEY_A))
			cam->position -= cam->right * (float)(time_delta * cam->speed);
        if (Input::is_key_pressed(GLFW_KEY_D))
			cam->position += cam->right * (float)(time_delta * cam->speed);
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
// 		Transform &ot = get_component<Transform>();
//         ot.position += ot.front * (float)(e.get_offset().y / 5. * ot.speed);
    }

    void on_cursor_moved_event(MouseMovedEvent &e)
    {
//         if (Input::is_imgui_window_hovered())
//             return;

		vec2 cursor_shift = e.get_cursor_pos() - cursor_pos;
//
//         if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_3))
//             get_component<Transform>().rotate_about_target(cursor_shift);

        if (Input::is_key_pressed(GLFW_KEY_LEFT_CONTROL))
		{
			shared_ptr<Camera> cam = get_component<CameraComponent>().camera;

			mat4 rot = rotate(-cursor_shift.x * cam->rotation_speed, cam->up);
			cam->front = vec3( rot *vec4(cam->front, 1.0));
			cam->update();

			rot = rotate(-cursor_shift.y * cam->rotation_speed, cam->right);
			cam->front = vec3( rot *vec4(cam->front, 1.0));
			cam->update();

// 			cam->rotation.x += cursor_shift.y * cam->rotation_speed;
// 			cam->rotation.z += cursor_shift.x * cam->rotation_speed;

		}

		cursor_pos= e.get_cursor_pos();
    }

};

#endif /* CAMERACONTROLLER_H_ */
