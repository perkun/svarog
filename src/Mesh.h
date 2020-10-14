#ifndef MESH_H_
#define MESH_H_


#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

class Mesh
{
public:
	Mesh();
	~Mesh();
	uint id;
	void draw();

private:
	void create_vao();

	enum {
		POSITIONS,
		INDICES,
		NUM_BUFFS
	};

	GLuint vertex_array_object;
	GLuint vertex_buffers[NUM_BUFFS];
};

#endif
