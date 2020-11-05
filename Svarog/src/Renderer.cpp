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
