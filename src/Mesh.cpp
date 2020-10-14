#include "Mesh.h"

Mesh::Mesh()
{
	create_vao();
}

Mesh::~Mesh()
{
//     glDeleteVertexArrays(1, &vertex_array_object);
//     glDeleteBuffers(NUM_BUFFERS, vertex_array_buffers);

}

void Mesh::create_vao()
{
	float positions[] = {
		 -0.5,  0.5, 0.0,
		  0.5,  0.5, 0.0,
		 -0.5, -0.5, 0.0,
		  0.5, -0.5, 0.0
	};

	unsigned int faces[6] = {0, 1, 2,
							 1, 3, 2};


	// VAO
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

	// generate buffers
	glGenBuffers(NUM_BUFFS, vertex_buffers);

	// VBO for vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITIONS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO

	// normals


	// textures

	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
}




void Mesh::draw()
{
	// DRAWING
	glBindVertexArray(vertex_array_object);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

