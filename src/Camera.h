#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

class Camera
{
public:
	Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect,
			float zNear, float zFar);

	~Camera();

	mat4 get_perspective();
	mat4 get_view();

	bool is_moving_forwards = false;
	bool is_moving_backwards = false;
	bool is_moving_left = false;
	bool is_moving_right = false;

	void move_forwards(double);
	void move_backwards(double);
	void move_left(double);
	void move_right(double);

	void pitch(float);
	void yaw(float);

	void move(double);


	vec3 position;
	vec3 target;
	vec3 forward;

	double speed = 0.6; // world space units per second
	double rotation_speed = 0.001;  // radians / sec

	float fov, aspect, z_near, z_far;

private:
	mat4 perspective;
	mat4 view;

	vec3 up, right, front;

	void update();



};

#endif /* CAMERA_H_ */
