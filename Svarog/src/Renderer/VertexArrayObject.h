#ifndef VERTEXARRAYOBJECT_H_
#define VERTEXARRAYOBJECT_H_

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "VertexLayout.h"

using namespace std;

// Currently only one vertex buffer
// TODO: multiple vertex buffer objects

class VertexArrayObject
{
public:
    VertexArrayObject(VertexLayout vl);
	~VertexArrayObject();
	VertexArrayObject(const VertexArrayObject &other);

	void create();

	VertexLayout vertices_layout;

	GLuint vao_id;
	GLuint vbo_id;
	GLuint idxs_id;

	int num_draw_elements;
private:
	bool created = false;
};

#endif /* RENDERER/VERTEXARRAYOBJECT_H_ */
