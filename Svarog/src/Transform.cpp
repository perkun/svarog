#include "svpch.h"
#include "Transform.h"

Transform::Transform()
{
	position = vec3(0.0);
	scale = vec3(1.0);
	alpha = beta = gamma = 0.0;

	up = vec3(0., 0., 1.);
	front = vec3(1., 0., 0.);
}

Transform::~Transform() {}


void Transform::update_local()
{
	local =
// 	 	 glm::translate(position) * get_rotation_matrix() * glm::scale(scale);
	 	 glm::translate(position) * get_rotation_matrix_313() * glm::scale(scale);
}

// void Transform::set_rotation_alpha_beta_gamma()
// {
// 	rotation = get_rotation_matrix_313();
// }


mat4 Transform::get_local_tansform()
{
	update_local();
	return local;
}


mat4 Transform::get_world_tansform()
{
	return world;
}

mat4 Transform::get_rotation_matrix()
{
	// roll
// 	mat4 roll_mat(1.0);
	mat4 roll_mat = rotate(-(float)(roll_angle), vec3(0., 1., 0.));
// 	right = vec3(roll_mat * vec4(right, 1.0));
// 	up = normalize(cross(right, front));

	// pitch
// 	mat4 pitch_mat(1.0);
	mat4 pitch_mat = rotate(-(float)(pitch_angle), vec3(1., 0., 0.));

	// yaw
// 	mat4 yaw_mat(1.0);
	mat4 yaw_mat = rotate(-(float)(yaw_angle), vec3(0., 0., 1.));


	front = vec3(yaw_mat * pitch_mat * roll_mat * vec4(front, 1.0));
	right = vec3(yaw_mat * pitch_mat * roll_mat * vec4(right, 1.0));
	up = vec3(yaw_mat * pitch_mat * roll_mat * vec4(up, 1.0));

	return yaw_mat * pitch_mat * roll_mat;
}

mat4 Transform::get_rotation_matrix_313()
{
	/**
	 * zwróci macierz obrotu o kąty Eulera 313 (ZXZ)
	 */
	return
		rotate(alpha, vec3(0.0, 0.0, 1.0)) *
		rotate(beta,  vec3(1.0, 0.0, 0.0)) *
		rotate(gamma, vec3(0.0, 0.0, 1.0));
}




void Transform::update_target(vec3 new_target)
{
// 	set up, right and front
// 	init(new_target);

	front = normalize(new_target - position);
	update();
}


void Transform::update()
{
	right = normalize(cross(front, up));
	right.z = 0.0;
	up = normalize(cross(right, front));

// 	view = glm::lookAt(position, position + front, up);
}



mat4 Transform::get_view()
{
	return glm::lookAt(position, position + front, up);
}

void Transform::move_forwards(double time_delta)
{
	position += front * (float)(time_delta * speed);
}


void Transform::move_backwards(double time_delta)
{
	position -= front * (float)(time_delta * speed);
}

void Transform::move_up(double time_delta)
{
	position += up * (float)(time_delta * speed);
}

void Transform::move_down(double time_delta)
{
	position -= up * (float)(time_delta * speed);
}


void Transform::move_left(double time_delta)
{
	position -= right * (float)(time_delta * speed);
}

void Transform::move_right(double time_delta)
{
	position += right * (float)(time_delta * speed);
}



void Transform::pan_right(double time_delta)
{
	position += vec3(vec2(right * (float)(time_delta * speed)), 0.);
}


void Transform::pan_left(double time_delta)
{
	position -= vec3(vec2(right * (float)(time_delta * speed)), 0.);
}

void Transform::pan_forwards(double time_delta)
{
	position += vec3(vec2(front * (float)(time_delta * speed)), 0.);
}


void Transform::pan_backwards(double time_delta)
{
	position -= vec3(vec2(front * (float)(time_delta * speed)), 0.);
}



void Transform::pitch(float shift)
{
	// rotate up/down obout right vector
	pitch_angle += shift * rotation_speed;
	mat4 tmp_rot = rotate(-(float)(rotation_speed * shift), right);
// // 	rotation = tmp_rot * rotation;
//
	front = vec3(tmp_rot * vec4(front, 1.0));
	update();
}


void Transform::yaw(float shift)
{
	// rotate left/right obout up vector
	yaw_angle += shift * rotation_speed;
	mat4 tmp_rot = rotate(-(float)(rotation_speed * shift), up);
// // 	rotation = tmp_rot * rotation;
//
	front = vec3(tmp_rot * vec4(front, 1.0));
	update();

}

void Transform::roll(float shift)
{
	// rotate about front vector
	roll_angle += roll_angle * rotation_speed;
	right = vec3(rotate(-(float)(rotation_speed * shift), front) * vec4(right, 1.0));
	up = normalize(cross(right, front));
}

void Transform::rotate_about_origin(vec2 cursor_shift,
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

vec3 Transform::calculate_intersection_point(vec3 plane_point, vec3 plane_normal)
{
	float d = dot(plane_point - position, vec3(0., 0., 1.)) / dot(front, vec3(0., 0., 1.));
	return position + (front * d);
}

void Transform::rotate_about_target(vec2 cursor_shift)
{
	vec3 intersection_point = calculate_intersection_point(vec3(0.), vec3(0., 0., 1.));
	position = position - intersection_point;

	rotate_about_origin(cursor_shift, -M_PI_2 + 0.2, M_PI_2 - 0.2);

	position = position + intersection_point;

	update();
}

// void Transform::move(double time_delta)
// {
//     bool refresh = false;
//
//     if (is_moving_forwards)
//     {
//         move_forwards(time_delta);
//         refresh = true;
//     }
//
//     if (is_moving_backwards)
//     {
//         move_backwards(time_delta);
//         refresh = true;
//     }
//
//     if (is_panning_left)
//     {
//         pan_left(time_delta);
//         refresh = true;
//     }
//
//     if (is_panning_right)
//     {
//         pan_right(time_delta);
//         refresh = true;
//     }
//
//     if (is_panning_forwards)
//     {
//         pan_forwards(time_delta);
//         refresh = true;
//     }
//
//     if (is_panning_backwards)
//     {
//         pan_backwards(time_delta);
//         refresh = true;
//     }
//
//
//     if (is_moving_left)
//     {
//         move_left(time_delta);
//         refresh = true;
//     }
//
//     if (is_moving_right)
//     {
//         move_right(time_delta);
//         refresh = true;
//     }
//
//     if (is_moving_up)
//     {
//         move_up(time_delta);
//         refresh = true;
//     }
//
//     if (is_moving_down)
//     {
//         move_down(time_delta);
//         refresh = true;
//     }
//
//     if (refresh)
//     {
//         update();
//     }
// }
