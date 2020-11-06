#ifndef RENDERER_H_
#define RENDERER_H_

#include "Mesh.h"
#include "Shader.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class Renderer
{
    public:
    Renderer();
    ~Renderer();

    void draw(Mesh &mesh, Shader &shader);
	void clear();
	void clear(float, float, float, float);

	void enable_blend();
	void disable_blend();

private:
	bool blend = false;
};

#endif /* RENDERER_H_ */
