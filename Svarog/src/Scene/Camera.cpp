#include "svpch.h"
#include "Camera.h"






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


mat4 OrthogonalCamera::get_perspective()
{
	view_box_left = -size_x / 2.0;
	view_box_right = size_x / 2.0;

	view_box_top = size_x / 2.0 / aspect;
	view_box_bottom = -size_x / 2.0 / aspect;

    return glm::ortho(view_box_left, view_box_right, view_box_bottom,
                             view_box_top, view_box_z_near, view_box_z_far);
}
