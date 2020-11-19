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
	~Camera();

	void init(vec3 target);
	mat4 get_perspective();

	void update();

	float aspect;

protected:
	mat4 perspective;

	float view_box_z_near, view_box_z_far;

private:
	virtual void calculate_perspective_matrix();

};







class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspect, float zNear, float zFar);

    ~PerspectiveCamera();

private:
    virtual void calculate_perspective_matrix() override;

	float fov;
};



class OrthogonalCamera : public Camera
{
public:
    OrthogonalCamera(float size_x, float aspect, float zNear, float zFar);
    ~OrthogonalCamera();

private:
    virtual void calculate_perspective_matrix() override;
	float view_box_top, view_box_bottom, view_box_left, view_box_right;
	float size_x;
};

#endif /* CAMERA_H_ */
