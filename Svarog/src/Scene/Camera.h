#ifndef CAMERA_H_
#define CAMERA_H_

using namespace std;
using namespace glm;

class Camera
{
public:
	Camera();
	virtual mat4 get_perspective() = 0;

	mat4 get_view();
	void update_target(vec3 new_target);
	void update();
	void update_near_far(float near, float far);

	float aspect;
	vec3 position, front, up, right;
	float speed = 10., rotation_speed = 0.0003;
	float view_box_z_near, view_box_z_far;

protected:
	void calculate_view();

	mat4 view;
};


class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspect, float zNear, float zFar);
    ~PerspectiveCamera();

	virtual mat4 get_perspective() override;

protected:
	float fov;
};


class OrthograficCamera : public Camera
{
public:
    OrthograficCamera(float size_x, float aspect, float zNear, float zFar);
    ~OrthograficCamera() {}

	virtual mat4 get_perspective() override;

	float size_x;

protected:
	float view_box_top, view_box_bottom, view_box_left, view_box_right;
};

#endif /* CAMERA_H_ */
