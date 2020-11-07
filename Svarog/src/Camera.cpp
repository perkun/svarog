#include "Camera.h"


Camera::Camera(const glm::vec3 pos, glm::vec3 target, float fov, float aspect,
               float zNear, float zFar)
    : position(pos), fov(fov), aspect(aspect), z_near(zNear),
      z_far(zFar)
{
	up = vec3(0, 0, 1);

	front = normalize(target - position);
	right = normalize(cross(front, up));
	up = normalize(cross(right, front));


	update();
}


Camera::~Camera()
{
}


mat4 Camera::get_perspective()
{
	return perspective;
}


mat4 Camera::get_view()
{
	return view;
}

void Camera::move_forwards(double time_delta)
{
	position += front * (float)(time_delta * speed);
}


void Camera::move_backwards(double time_delta)
{
	position -= front * (float)(time_delta * speed);
}


void Camera::pan_forwards(double time_delta)
{
	position += vec3(vec2(front * (float)(time_delta * speed)), 0.);
}


void Camera::pan_backwards(double time_delta)
{
	position -= vec3(vec2(front * (float)(time_delta * speed)), 0.);
}


void Camera::move_left(double time_delta)
{
	position -= right * (float)(time_delta * speed);
}

void Camera::move_right(double time_delta)
{
	position += right * (float)(time_delta * speed);
}


void Camera::pitch(float shift_y)
{
	front = vec3(rotate(-(float)(rotation_speed * shift_y), right) * vec4(front, 1.0));
	update();
}


void Camera::yaw(float shift_x)
{

	front = vec3(rotate(-(float)(rotation_speed * shift_x), up) * vec4(front, 1.0));
	update();

}

void Camera::rotate_about_origin(vec2 cursor_shift,
                                 float min_height = -M_PI_2 + 0.2,
                                 float max_height = M_PI_2 - 0.2)
{
  float r = glm::length(position);
  float azimuth = atan2(position.y, position.x);
  float height = asin(position.z / r);

  azimuth -= cursor_shift.x * rotation_speed;
  height += cursor_shift.y  * rotation_speed;

  height = clamp(height, min_height, max_height);

  position.x = r * cos(height) * cos(azimuth);
  position.y = r * cos(height) * sin(azimuth);
  position.z = r * sin(height);

  front = -normalize(position);

  update();
}

vec3 Camera::calculate_intersection_point(vec3 plane_point, vec3 plane_normal)
{
	float d = dot(plane_point - position, vec3(0., 0., 1.)) / dot(front, vec3(0., 0., 1.));
	return position + (front * d);
}

void Camera::rotate_about_target(vec2 cursor_shift)
{
	vec3 intersection_point = calculate_intersection_point(vec3(0.), vec3(0., 0., 1.));
	position = position - intersection_point;

	rotate_about_origin(cursor_shift, 0.2, M_PI_2 - 0.2);

	position = position + intersection_point;

	update();
}

void Camera::move(double time_delta)
{
    bool refresh = false;

    if (is_moving_forwards)
    {
        move_forwards(time_delta);
        refresh = true;
    }

    if (is_moving_backwards)
    {
        move_backwards(time_delta);
        refresh = true;
    }

    if (is_panning_forwards)
    {
        pan_forwards(time_delta);
        refresh = true;
    }

    if (is_panning_backwards)
    {
        pan_backwards(time_delta);
        refresh = true;
    }

    if (is_moving_left)
    {
        move_left(time_delta);
        refresh = true;
    }

    if (is_moving_right)
    {
        move_right(time_delta);
        refresh = true;
    }

    if (refresh)
    {
        update();
    }
}


void Camera::update()
{


	right = normalize(cross(front, up));
	right.z = 0.0;
	up = normalize(cross(right, front));


	view = glm::lookAt(position, position + front, up);
	perspective = glm::perspective(fov, aspect, z_near, z_far);
}

