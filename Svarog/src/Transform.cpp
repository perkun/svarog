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
