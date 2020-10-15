#ifndef MESH_H_
#define MESH_H_

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;


class IndexModel
{
public:
	IndexModel(string filename);
	~IndexModel();

	void load_obj(string filename);

// 	int positions_size = 0;
// 	int normals_size = 0;
// 	int texture_size = 0;
//
// 	int position_ids_size = 0;
// 	int normal_ids_size = 0;
// 	int texture_ids_size = 0;
//
// 	float *positions = NULL;
// 	float *normals = NULL;
// 	float *texture_coords = NULL;
//
// 	unsigned int *position_ids = NULL;
// 	unsigned int *normal_ids = NULL;
// 	unsigned int *texture_ids = NULL;

	float *vertex_pos = NULL;
	float *texture_pos = NULL;
	float *normal_pos = NULL;
	unsigned int *indices = NULL;

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
