#include "Camera.h"

Camera::Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect,
               float zNear, float zFar)
    : position(pos), target(targ), fov(fov), aspect(aspect), z_near(zNear),
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


void Camera::move(double time_delta)
{
	bool refresh = false;

	if (is_moving_forwards) {
		move_forwards(time_delta);
		refresh = true;
	}

	if (is_moving_backwards)
	{
		move_backwards(time_delta);
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

	if (refresh) {
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

