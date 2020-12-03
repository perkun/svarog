#ifndef VERTEXARRAYOBJECT_H_
#define VERTEXARRAYOBJECT_H_

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "VertexLayout.h"
#include "IndexedModel.h"

using namespace std;

// Currently only one vertex buffer
// TODO: multiple vertex buffer objects

class VertexArrayObject
{
public:
//     VertexArrayObject(VertexLayout vl);
	VertexArrayObject() {}
	VertexArrayObject(IndexedModel idx_mod, bool b = false);
	~VertexArrayObject();
// 	VertexArrayObject(const VertexArrayObject &other);

	void create(IndexedModel&);

// 	VertexLayout vertices_layout;

	GLuint vao_id;
	GLuint vbo_id;
	GLuint idxs_id;

	int num_draw_elements;
	bool blend = false;
	unsigned int draw_type = GL_TRIANGLES;

private:
	bool created = false;
	int usage = GL_STATIC_DRAW;
};


class DynamicVertexArrayObject : public VertexArrayObject
{
public:
	DynamicVertexArrayObject();
	DynamicVertexArrayObject(IndexedModel idx_mod);
	~DynamicVertexArrayObject();

	void update_buffer(const IndexedModel &idx_mod);
private:
// 	size_t initial_size;
	bool created = false;
	int usage = GL_DYNAMIC_DRAW;

};

#endif /* RENDERER/VERTEXARRAYOBJECT_H_ */
