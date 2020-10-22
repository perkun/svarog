#include "Camera.h"

Camera::Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect,
               float zNear, float zFar)
    : position(pos), target(targ), fov(fov), aspect(aspect), z_near(zNear),
      z_far(zFar)
{
	up = vec3(0, 0, 1);

	view = glm::lookAt(position, target, up);
	perspective = glm::perspective(fov, aspect, z_near, z_far);

}


Camera::~Camera()
{
}


mat4 Camera::get_perspective()
{
	perspective = glm::perspective(fov, aspect, z_near, z_far);
	return perspective;
}


mat4 Camera::get_view()
{
	view = glm::lookAt(position, target, up);
	return view;
}

void Camera::move_forwards(double time_delta)
{
	position.y += time_delta * speed;
}


void Camera::move_backwards(double time_delta)
{
	position.y -= time_delta * speed;
}


void Camera::move_left(double time_delta)
{
	position.x -= time_delta * speed;
}

void Camera::move_right(double time_delta)
{
	position.x += time_delta * speed;
}


void Camera::move(double time_delta)
{
	if (is_moving_forwards)
		move_forwards(time_delta);

	if (is_moving_backwards)
		move_backwards(time_delta);

	if (is_moving_left)
		move_left(time_delta);

	if (is_moving_right)
		move_right(time_delta);
}
