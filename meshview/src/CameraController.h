#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"
#include "Camera.h"


using namespace std;

class CameraController : public ScriptableEntity
{
public:
    ~CameraController()
    {
    }

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
        ASSERT(has_component<CameraComponent>(),
               "CameraController ERROR: Entity does not have Camera Component");
    }

    virtual void on_event(Event &e) override
    {
        EventDispacher dispatcher(e);

//         dispatcher.dispatch<MouseMovedEvent>(bind(
//             &CameraController::on_cursor_moved_event, this, placeholders::_1));

        dispatcher.dispatch<MouseScrolledEvent>(
            bind(&CameraController::on_mouse_scrolled_event, this,
                 placeholders::_1));
    }

    void on_mouse_scrolled_event(MouseScrolledEvent &e)
    {
        auto cam = dynamic_pointer_cast<OrthograficCamera>(
            get_component<CameraComponent>().camera);

        cam->size_x -= e.get_offset().y / 5;
    }

    void on_cursor_moved_event(MouseMovedEvent &e)
    {
    }
};

#endif
