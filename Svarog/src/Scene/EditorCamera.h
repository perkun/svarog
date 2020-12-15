#ifndef SCENECAMERA_H_
#define SCENECAMERA_H_

#include "Camera.h"

using namespace std;

class EditorCamera : public PerspectiveCamera
{
public:
    EditorCamera(float fov, float aspect, float zNear, float zFar);
	~EditorCamera();

};

#endif /* SCENECAMERA_H_ */
