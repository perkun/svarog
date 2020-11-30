#include "Renderer.h"


namespace Renderer {


// private:
bool blend = false;
// vec4 clear_color = vec4(0., 0., 0., 1.);
////


void init()
{
	glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
//     glEnable(GL_CULL_FACE);
//     glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glPointSize(10.0f);

    // 	glEnable(GL_BLEND);
    // 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void enable_blend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void disable_blend()
{
	glDisable(GL_BLEND);
}

void set_blend(bool mesh_blend)
{
	if (mesh_blend)
	{
		if (!blend)
		{
			enable_blend();
			blend = true;
		}
	}
	else
	{
		if (blend)
		{
			disable_blend();
			blend = false;
		}
	}

}

void draw(VertexArrayObject *vao, Shader *shader)
{
	shader->bind();
	set_blend(vao->blend);

	glBindVertexArray(vao->vao_id);
	glDrawElements(vao->draw_type, vao->num_draw_elements, GL_UNSIGNED_INT, NULL);
}

void clear()
{
// 	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b ,a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void set_viewport(int x, int y, int size_x,
                  int size_y)
{
	glViewport(x, y, size_x, size_y);
}


void bind_default_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Renderer
