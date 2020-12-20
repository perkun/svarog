#ifndef RENDERER_H_
#define RENDERER_H_

#include "Shader.h"
#include "VertexArrayObject.h"

using namespace std;
using namespace glm;

namespace Renderer
{
	void init();
    void draw(shared_ptr<VertexArrayObject> vao, Shader *shader);
	void clear();
	void clear(vec4);
	void clear(float, float, float, float);
	void set_viewport(int, int, int, int);
	void set_line_width(float width);
	void bind_default_framebuffer();

	void enable_blend();
	void disable_blend();
};

#endif /* RENDERER_H_ */
