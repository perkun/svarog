#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"
#include "Camera.h"
#include "Components.h"
#include "Application.h"


using namespace std;

class CameraController : public ScriptableEntity
{
public:
    ~CameraController()
    {
    }

protected:
    vec2 last_cursor_pos;
    float mouse_sensitivity = 1. / 900;
    vec2 cursor_pos;

    virtual void on_create() override
    {
    }
    virtual void on_destroy() override
    {
    }

    virtual void on_update(double time_delta) override
    {
        ASSERT(has_component<CameraComponent>(),
               "CameraController ERROR: Entity does not have Camera Component");
    }

    virtual void on_event(Event &e) override
    {
        EventDispacher dispatcher(e);

        dispatcher.dispatch<MouseMovedEvent>(bind(
            &CameraController::on_cursor_moved_event, this, placeholders::_1));

        dispatcher.dispatch<MouseScrolledEvent>(
            bind(&CameraController::on_mouse_scrolled_event, this,
                 placeholders::_1));
    }

    void on_mouse_scrolled_event(MouseScrolledEvent &e)
    {
        auto cam = dynamic_pointer_cast<OrthograficCamera>(
            get_component<CameraComponent>().camera);

        cam->fov -= e.get_offset().y / 5;
        if (cam->fov < 0.1)
            cam->fov = 0.1;
    }

    void on_cursor_moved_event(MouseMovedEvent &e)
    {
        vec2 cursor_shift = e.get_cursor_pos() - last_cursor_pos;
        if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_2))
        {
            // 			Transform &t = get_component<Transform>();
            // 			t.position.x -= cursor_shift.x *
            // mouse_sensitivity; 			t.position.z += cursor_shift.y *
            // mouse_sensitivity;


            CameraComponent &cc = get_component<CameraComponent>();

			float sf = 1920. * 0.47 / Application::get_window()->width;

            cc.camera->position.x -=
                cursor_shift.x * mouse_sensitivity * cc.camera->fov * sf;
            cc.camera->position.z +=
                cursor_shift.y * mouse_sensitivity * cc.camera->fov* sf;
        }

        last_cursor_pos = e.get_cursor_pos();
    }
};

#endif
