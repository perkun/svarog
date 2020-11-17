#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::init(vec3 target)
{
	up = vec3(0, 0, 1);
	front = normalize(target - position);
	up = normalize(cross(right, front));

	if ( length(cross(front, up)) < 0.0001)
		up = vec3( rotate((float)(-M_PI_2), vec3(1., 0., 0.)) * vec4(up, 1.));
		up = vec3(0., 1., 0.);

	update();
}

void Camera::update()
{
	right = normalize(cross(front, up));
	right.z = 0.0;
	up = normalize(cross(right, front));

	view = glm::lookAt(position, position + front, up);
	calculate_perspective_matrix();
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

void Camera::move_up(double time_delta)
{
	position += up * (float)(time_delta * speed);
}

void Camera::move_down(double time_delta)
{
	position -= up * (float)(time_delta * speed);
}


void Camera::move_left(double time_delta)
{
	position -= right * (float)(time_delta * speed);
}

void Camera::move_right(double time_delta)
{
	position += right * (float)(time_delta * speed);
}



void Camera::pan_right(double time_delta)
{
	position += vec3(vec2(right * (float)(time_delta * speed)), 0.);
}


void Camera::pan_left(double time_delta)
{
	position -= vec3(vec2(right * (float)(time_delta * speed)), 0.);
}

void Camera::pan_forwards(double time_delta)
{
	position += vec3(vec2(front * (float)(time_delta * speed)), 0.);
}


void Camera::pan_backwards(double time_delta)
{
	position -= vec3(vec2(front * (float)(time_delta * speed)), 0.);
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

    if (is_panning_left)
    {
        pan_left(time_delta);
        refresh = true;
    }

    if (is_panning_right)
    {
        pan_right(time_delta);
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

    if (is_moving_up)
    {
        move_up(time_delta);
        refresh = true;
    }

    if (is_moving_down)
    {
        move_down(time_delta);
        refresh = true;
    }

    if (refresh)
    {
        update();
    }
}

void Camera::calculate_perspective_matrix()
{
//     perspective = glm::perspective(fov, aspect, z_near, z_far);
}


// PERSPECTIVE CAMERA

PerspectiveCamera::PerspectiveCamera(const glm::vec3 pos, glm::vec3 target,
                                     float fov, float aspect, float zNear,
                                     float zFar)

{
	this->position = pos;
	this->fov = fov;
	this->aspect = aspect;
	this->view_box_z_near = zNear;
	this->view_box_z_far = zFar;

    init(target);
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

OrthogonalCamera::OrthogonalCamera(const glm::vec3 pos, glm::vec3 target,
                                   float size_x, float aspect,
                                   float view_box_zNear, float view_box_zFar)
{
    this->position = pos;
    this->view_box_z_near = view_box_zNear;
    this->view_box_z_far = view_box_zFar;
	this->aspect = aspect;
	this->size_x = size_x;

	view_box_left = -size_x / 2.0;
	view_box_right = size_x / 2.0;

	view_box_top = size_x / 2.0 / aspect;
	view_box_bottom = -size_x / 2.0 / aspect;

//     this->view_box_top = view_box_top;
//     this->view_box_bottom = view_box_bottom;

//     this->view_box_left = view_box_left;
//     this->view_box_right = view_box_right;

	aspect = (view_box_right - view_box_left) / (view_box_top - view_box_bottom);

    init(target);
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
