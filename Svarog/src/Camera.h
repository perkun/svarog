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
	Camera();
// 	Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect,
// 			float zNear, float zFar);
// 	Camera(const glm::vec3 pos, glm::vec3 targ, float left, float right,
//    		float bottom, float top, float zNear, float zFar);
	~Camera();

	void init(vec3 target);

	mat4 get_perspective();
	mat4 get_view();



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
	void move(double);
	void update();

	vec3 calculate_intersection_point(vec3 plane_point, vec3 plane_normal);

	vec3 position;
	vec3 front;
	float aspect;

	double speed = 0.6; // world space units per second
	double rotation_speed = 0.001;  // radians / sec

	bool active = false;

protected:
	mat4 perspective;
	mat4 view;
	vec3 up, right;

	float view_box_z_near, view_box_z_far;

private:
// 	void calculate_perspective_matrix();
	virtual void calculate_perspective_matrix();

};


class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const glm::vec3 pos, glm::vec3 targ, float fov,
                      float aspect, float zNear, float zFar);

    ~PerspectiveCamera();

private:
    virtual void calculate_perspective_matrix() override;

	float fov;
};



class OrthogonalCamera : public Camera
{
public:
    OrthogonalCamera(const glm::vec3 pos, glm::vec3 target, float size_x,
					 float aspect, float zNear,
                     float zFar);
    ~OrthogonalCamera();

private:
    virtual void calculate_perspective_matrix() override;
	float view_box_top, view_box_bottom, view_box_left, view_box_right;
	float size_x;
};

#endif /* CAMERA_H_ */
