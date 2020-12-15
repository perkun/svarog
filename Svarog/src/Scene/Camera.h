#ifndef CAMERA_H_
#define CAMERA_H_


using namespace std;
using namespace glm;

class Camera
{
public:
	virtual mat4 get_perspective() = 0;
	float aspect;

protected:
	float view_box_z_near, view_box_z_far;
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



class OrthogonalCamera : public Camera
{
public:
    OrthogonalCamera(float size_x, float aspect, float zNear, float zFar);
    ~OrthogonalCamera();

	virtual mat4 get_perspective() override;

protected:
	float view_box_top, view_box_bottom, view_box_left, view_box_right;
	float size_x;
};

#endif /* CAMERA_H_ */
