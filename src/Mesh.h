#ifndef MESH_H_
#define MESH_H_

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

using namespace std;


class IndexModel
{
public:
	IndexModel(string filename);
	~IndexModel();

	void load_obj(string filename);

private:
	float *positions;
	float *normals;
	float *texture_coords;

	int *position_ids;
	int *normal_ids;
	int *texture_ids;
};


class Mesh
{
public:
	Mesh(string filename);
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
