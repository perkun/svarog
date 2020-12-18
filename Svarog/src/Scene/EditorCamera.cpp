#include "svpch.h"
#include "EditorCamera.h"


EditorCamera::EditorCamera() : PerspectiveCamera(0., 0., 0., 0.)
{
	up = vec3(0., 0., 1.);
	front = vec3(1., 0., 0.);

    position = vec3(6.3, -3., 5.12);
    update_target(vec3(0., 0., 0.));
    speed = 8.;

}

EditorCamera::EditorCamera(float fov, float aspect, float zNear, float zFar, vec3 position)
 : PerspectiveCamera(fov, aspect, zNear, zFar)
{
	up = vec3(0., 0., 1.);
	front = vec3(1., 0., 0.);

    this->position = position;
    update_target(vec3(0., 0., 0.));
    speed = 8.;
}


EditorCamera::~EditorCamera()
{
}



void EditorCamera::on_update(double time_delta)
{
//     if (Input::is_key_pressed(GLFW_KEY_W))
//         pan_forwards(time_delta);
//     if (Input::is_key_pressed(GLFW_KEY_S))
//         pan_backwards(time_delta);
//     if (Input::is_key_pressed(GLFW_KEY_A))
//         pan_left(time_delta);
//     if (Input::is_key_pressed(GLFW_KEY_D))
//         pan_right(time_delta);
}

void EditorCamera::on_event(Event &e)
{
	EventDispacher dispatcher(e);

	dispatcher.dispatch<MouseMovedEvent>(bind(
				&EditorCamera::on_cursor_moved_event, this, placeholders::_1));

	dispatcher.dispatch<MouseScrolledEvent>(bind(
				&EditorCamera::on_mouse_scrolled_event, this, placeholders::_1));

}

void EditorCamera::on_resize(float width, float height)
{
	aspect = width / (float)height;
}


void EditorCamera::on_cursor_moved_event(MouseMovedEvent &e)
{
    vec2 cursor_shift = e.get_cursor_pos() - cursor_pos;

    if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_3))
	{
        rotate_about_target(cursor_shift);
	}

    if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_2))
    {
        pan_forwards(cursor_shift.y / 600.);
        pan_left(cursor_shift.x / 600.);
    }

    cursor_pos = e.get_cursor_pos();
}


void EditorCamera::on_mouse_scrolled_event(MouseScrolledEvent &e)
{
	position += front * (float)(e.get_offset().y / 5. * speed);
}


mat4 EditorCamera::get_view()
{
	return glm::lookAt(position, position + front, up);
}


void EditorCamera::update_target(vec3 new_target)
{
	front = normalize(new_target - position);
	update();
}

void EditorCamera::update()
{
	right = normalize(cross(front, up));
	right.z = 0.0;
	up = normalize(cross(right, front));
}

void EditorCamera::calculate_speed()
{
    speed = glm::length(
        calculate_intersection_point(vec3(0.), vec3(0., 0., 1.)) - position);
    speed *= 1920. * 0.47 / Application::get_window()->width;
}

void EditorCamera::pan_right(double time_delta)
{
	calculate_speed();
	position += vec3(vec2(right * (float)(time_delta * speed)), 0.);
}


void EditorCamera::pan_left(double time_delta)
{
	calculate_speed();
	position -= vec3(vec2(right * (float)(time_delta * speed)), 0.);
}

void EditorCamera::pan_forwards(double time_delta)
{
	calculate_speed();
	position += vec3(vec2(front * (float)(time_delta * speed)), 0.);
}


void EditorCamera::pan_backwards(double time_delta)
{
	calculate_speed();
	position -= vec3(vec2(front * (float)(time_delta * speed)), 0.);
}



void EditorCamera::rotate_about_origin(vec2 cursor_shift,
                                 float min_height = -M_PI_2 + 0.2,
                                 float max_height = M_PI_2 - 0.2)
{
  float r = glm::length(position);
  float azimuth = atan2(position.y, position.x);
  float height = asin(position.z / r);

  azimuth -= cursor_shift.x * rotation_speed;
  height += cursor_shift.y  * rotation_speed;

  height = glm::clamp(height, min_height, max_height);

  position.x = r * cos(height) * cos(azimuth);
  position.y = r * cos(height) * sin(azimuth);
  position.z = r * sin(height);

  front = -normalize(position);

  update();
}


void EditorCamera::rotate_about_target(vec2 cursor_shift)
{
	vec3 intersection_point = calculate_intersection_point(vec3(0.), vec3(0., 0., 1.));
	position = position - intersection_point;

	rotate_about_origin(cursor_shift, -M_PI_2 + 0.2, M_PI_2 - 0.2);

	position = position + intersection_point;

	update();
}

vec3 EditorCamera::calculate_intersection_point(vec3 plane_point, vec3 plane_normal)
{
	float d = dot(plane_point - position, vec3(0., 0., 1.)) / dot(front, vec3(0., 0., 1.));
	return position + (front * d);
}
