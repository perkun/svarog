#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::draw(Mesh &mesh, Shader &shader)
{
	shader.bind();
	if (mesh.blend)
		enable_blend();
	else
		disable_blend();

	glBindVertexArray(mesh.vertex_array_object);
	glDrawElements(GL_TRIANGLES, mesh.num_triangles * 3, GL_UNSIGNED_INT, NULL);
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
