#ifndef MESH_H_
#define MESH_H_

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;


class IndexModel
{
public:
	IndexModel(string filename);
	~IndexModel();

	void load_obj(string filename);

	vector<vec3> ver;
	vector<vec3> tex;
	vector<vec3> nor;
	vector<uvec3> ver_idxs;

	unsigned int num_vertices;
	unsigned int num_indices;
};


class Mesh
{
public:
	Mesh(string filename);
	~Mesh();
	uint id;
	unsigned int num_triangles = 0;
	void draw();

private:
	void create_vao(IndexModel *indexed_model);

	enum {
		POSITIONS,
		TEXTURE_COORDS,
		NORMALS,
		INDICES,
		TEXTURE_INDICES,
		NORMAL_INDICES,
		NUM_BUFFS
	};

	GLuint vertex_array_object;
	GLuint vertex_buffers[NUM_BUFFS];
};

#endif
