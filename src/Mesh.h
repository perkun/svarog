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
	IndexModel(string filename, int mode);
	~IndexModel();

	void load_obj(string filename, int mode);

	vector<vec3> ver;
	vector<vec2> tex;
	vector<vec3> nor;
	vector<uvec3> ver_idxs;

	enum {
		PER_VERTEX,
		PER_FACE
	};

private:
	void calculate_per_vertex_normals(vector<vec3> &ver,
									  vector<uvec3> &ver_idxs,
								 	  vector<vec3> &nor);

	void calculate_per_face_normals(vector<vec3> &ver,
									vector<uvec3> &ver_idxs,
								 	vector<vec3> &nor);

	void produce_arrays_per_vertex(vector<vec3> &tmp_ver,
								   vector<vec2> &tmp_tex,
								   vector<vec3> &tmp_nor,
								   vector<uvec3> &ver_idxs,
								   vector<uvec3> &tex_idxs,
								   vector<uvec3> &nor_idxs);

	void produce_arrays_per_face(vector<vec3> &tmp_ver,
								 vector<vec2> &tmp_tex,
								 vector<vec3> &tmp_nor,
								 vector<uvec3> &ver_idxs,
								 vector<uvec3> &tex_idxs,
								 vector<uvec3> &nor_idxs);

};


class Mesh
{
public:
	enum {
		POSITIONS,
		TEXTURE_COORDS,
		NORMALS,
		INDICES,
		NUM_BUFFS
	};

	Mesh(string filename, int mode);
	~Mesh();
	uint id;
	unsigned int num_triangles = 0;

	GLuint vertex_array_object;
	GLuint vertex_buffers[NUM_BUFFS];
private:
	void create_vao(IndexModel *indexed_model);


};

#endif
