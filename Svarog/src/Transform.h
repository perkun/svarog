#ifndef TRANSFORM_H_
#define TRANSFORM_H_


#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

class Transform
{
public:
	Transform();
	~Transform();

	void update_local();
	mat4 get_local_tansform();
	mat4 get_world_tansform();
	mat4 get_view();

////////
	bool is_moving_forwards = false;
	bool is_moving_backwards = false;
	bool is_moving_up = false;
	bool is_moving_down = false;
	bool is_moving_left = false;
	bool is_moving_right = false;

	bool is_panning_right = false;
	bool is_panning_left = false;
	bool is_panning_forwards = false;
	bool is_panning_backwards = false;

	bool is_rotating = false;

	void update_target(vec3);

	void move_forwards(double);
	void move_backwards(double);
	void move_left(double);
	void move_right(double);
	void move_up(double);
	void move_down(double);

	void pan_left(double);
	void pan_right(double);
	void pan_forwards(double);
	void pan_backwards(double);

	void rotate_about_origin(vec2 cursor_shift, float min_height,
			float max_height);
	void rotate_about_target(vec2 cursor_shift);

	void pitch(float);
	void yaw(float);
	void roll(float);

	void move(double);
	void update();

	vec3 calculate_intersection_point(vec3 plane_point, vec3 plane_normal);

// 	vec3 position;
// 	vec3 front;

	double speed = 0.6; // world space units per second
	double rotation_speed = 0.001;  // radians / sec
///////





	mat4 local;
	mat4 world;
	mat4 view;

	vec3 up, front, right;

	float alpha, beta, gamma;
	vec3 scale, position;

	bool change_alpha = false;
	bool change_beta = false;
	bool change_gamma = false;

private:
	mat4 get_rotation_matrix_313();
};


#endif
