#ifndef LIGHT_H_
#define LIGHT_H_

#include "Framebuffer.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class Light
{
public:
    Light();
	~Light();

	vec4 color = vec4(1.0);
};

#endif /* LIGHT_H_ */
