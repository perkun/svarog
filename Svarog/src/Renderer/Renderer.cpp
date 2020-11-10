#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::set_blend(bool mesh_blend)
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

void Renderer::draw(VertexArrayObject *vao, Shader *shader)
{
	shader->bind();
	set_blend(vao->blend);

	glBindVertexArray(vao->vao_id);
	glDrawElements(GL_TRIANGLES, vao->num_draw_elements, GL_UNSIGNED_INT, NULL);
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b ,a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void Renderer::enable_blend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::disable_blend()
{
	glDisable(GL_BLEND);
}
