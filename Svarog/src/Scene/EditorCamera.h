#ifndef SCENECAMERA_H_
#define SCENECAMERA_H_

#include "Camera.h"
#include "Event.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"
#include "Input.h"

using namespace std;
using namespace glm;

class EditorCamera : public PerspectiveCamera
{
public:
	EditorCamera();
    EditorCamera(float fov, float aspect, float zNear, float zFar, vec3 pos);
	~EditorCamera();


	void on_event(Event &e);
	void on_update(double);
	void on_resize(float, float);

	mat4 get_view();
	void update();
	void update_target(vec3);

	void pan_left(double);
	void pan_right(double);
	void pan_forwards(double);
	void pan_backwards(double);

	void rotate_about_origin(vec2 cursor_shift, float min_height,
			float max_height);
	void rotate_about_target(vec2 cursor_shift);

private:
	void on_mouse_scrolled_event(MouseScrolledEvent &e);
	void on_cursor_moved_event(MouseMovedEvent &e);

	void calculate_speed();
	vec3 calculate_intersection_point(vec3 plane_point, vec3 plane_normal);

	vec3 up, front, right;
	vec3 position;
	float speed, rotation_speed = 0.002;

	vec2 cursor_pos = vec2(0.0);

};

#endif /* SCENECAMERA_H_ */
