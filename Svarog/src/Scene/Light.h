#ifndef LIGHT_H_
#define LIGHT_H_

#include "Framebuffer.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class Light
{
public:
    Light(shared_ptr<Framebuffer> fb, shared_ptr<Camera> cam);
	~Light();

	shared_ptr<Framebuffer> shadow_framebuffer;
	shared_ptr<Camera> camera;
};

#endif /* LIGHT_H_ */
