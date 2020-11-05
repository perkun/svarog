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

	mat4 local;
	mat4 world;

	float alpha, beta, gamma;
	vec3 scale, position;

	bool change_alpha = false;
	bool change_beta = false;
	bool change_gamma = false;

private:
	mat4 get_rotation_matrix_313();
};


#endif
