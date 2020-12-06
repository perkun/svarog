#include "svpch.h"
#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

// void Camera::init(vec3 target)
// {
// 	up = vec3(0., 0., 1.);
// 	front = normalize(target - position);
// // 	up = normalize(cross(right, front));
//
// // 	if ( length(cross(front, up)) < 0.0001)
// // 		up = vec3( rotate((float)(-M_PI_2), vec3(1., 0., 0.)) * vec4(up, 1.));
//
// // 	up = vec3(0., 1., 0.);
//
// 	update();
// }


mat4 Camera::get_perspective()
{
	return perspective;
}




void Camera::calculate_perspective_matrix()
{
//     perspective = glm::perspective(fov, aspect, z_near, z_far);
}

void Camera::update()
{
	calculate_perspective_matrix();
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

void PerspectiveCamera::calculate_perspective_matrix()
{
    perspective = glm::perspective(fov, aspect, view_box_z_near, view_box_z_far);
}


// ORTHOGONAL CAMERA
//

OrthogonalCamera::OrthogonalCamera(float size_x, float aspect,
                                   float view_box_zNear, float view_box_zFar)
{
    this->view_box_z_near = view_box_zNear;
    this->view_box_z_far = view_box_zFar;
	this->aspect = aspect;
	this->size_x = size_x;

	view_box_left = -size_x / 2.0;
	view_box_right = size_x / 2.0;

	view_box_top = size_x / 2.0 / aspect;
	view_box_bottom = -size_x / 2.0 / aspect;

	aspect = (view_box_right - view_box_left) / (view_box_top - view_box_bottom);
}

void OrthogonalCamera::calculate_perspective_matrix()
{
	view_box_left = -size_x / 2.0;
	view_box_right = size_x / 2.0;

	view_box_top = size_x / 2.0 / aspect;
	view_box_bottom = -size_x / 2.0 / aspect;

    perspective = glm::ortho(view_box_left, view_box_right, view_box_bottom,
                             view_box_top, view_box_z_near, view_box_z_far);
}
