#ifndef TRANSFORM_H_
#define TRANSFORM_H_


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

	void set_euler_lbg();

	float speed = 0.6; // world space units per second
	float rotation_speed = 0.002;  // radians / sec

	mat4 local;
	mat4 world;

	float lambda, beta, gamma;
	vec3 scale, position, rotation;

// 	mat4 get_rotation_matrix_313();
private:
	mat4 get_rotation_matrix();
};


#endif
