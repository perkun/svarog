#include "svpch.h"
#include "Camera.h"

Camera::Camera()
{
	up = vec3(0., 0., 1.);
	front = vec3(1., 0., 0.);

    position = vec3(0.0);
    update_target(vec3(1., 1., 1.));
}

mat4 Camera::get_view()
{
	calculate_view();
	return view;
}


void Camera::calculate_view()
{
	view = lookAt(position, position + front, up);
}


void Camera::update_target(vec3 new_target)
{
	if (length(new_target - position) == 0)
		return;
	front = normalize(new_target - position);
	update();
}

void Camera::update()
{
	right = normalize(cross(front, up));
	right.z = 0.0;
	up = normalize(cross(right, front));
}


void Camera::update_near_far(float near, float far)
{
	view_box_z_near = near;
	view_box_z_far = far;
}


// PERSPECTIVE CAMERA

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float zNear,
                                     float zFar)
{
	this->fov = fov;
	this->aspect = aspect;
	this->view_box_z_near = zNear;
	this->view_box_z_far = zFar;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

mat4 PerspectiveCamera::get_perspective()
{
    return glm::perspective(fov, aspect, view_box_z_near, view_box_z_far);
}




// ORTHOGONAL CAMERA


OrthograficCamera::OrthograficCamera(float size_x, float aspect,
                                   float view_box_zNear, float view_box_zFar)
{
    this->view_box_z_near = view_box_zNear;
    this->view_box_z_far = view_box_zFar;
	this->aspect = aspect;
	this->fov = size_x;

	view_box_left = -size_x / 2.0;
	view_box_right = size_x / 2.0;

	view_box_top = size_x / 2.0 / aspect;
	view_box_bottom = -size_x / 2.0 / aspect;

	aspect = (view_box_right - view_box_left) / (view_box_top - view_box_bottom);
}


mat4 OrthograficCamera::get_perspective()
{
	view_box_left = -fov / 2.0;
	view_box_right = fov / 2.0;

	view_box_top = fov / 2.0 / aspect;
	view_box_bottom = -fov / 2.0 / aspect;

    return glm::ortho(view_box_left, view_box_right, view_box_bottom,
                             view_box_top, view_box_z_near, view_box_z_far);
}
