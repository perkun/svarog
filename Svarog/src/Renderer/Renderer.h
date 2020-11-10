#ifndef RENDERER_H_
#define RENDERER_H_

#include <iostream>
#include <stdio.h>
// #include "Mesh.h"
#include "Shader.h"
#include "VertexArrayObject.h"

using namespace std;

class Renderer
{
    public:
    Renderer();
    ~Renderer();

    void draw(VertexArrayObject *vao, Shader *shader);
	void clear();
	void clear(float, float, float, float);

	void enable_blend();
	void disable_blend();

private:
	bool blend = false;
	void set_blend(bool mesh_blend);
};

#endif /* RENDERER_H_ */
