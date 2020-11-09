#ifndef INDEXEDMODEL_H_
#define INDEXEDMODEL_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "VertexLayout.h"


using namespace std;
using namespace glm;

enum class NormalIndexing
{
	PER_VERTEX,
	PER_FACE
};

class IndexedModel
{
public:
	IndexedModel() {}
	~IndexedModel() {}

	VertexLayout layout;
	vector<float> vertices;
	vector<unsigned int> indices;
};


class IndexedModelFile : public IndexedModel
{
public:
	IndexedModelFile() {}
	~IndexedModelFile() {}
	virtual void load(string filename, NormalIndexing mode) {}

protected:
	void produce_arrays_per_face(vector<vec3> &tmp_pos,
								 vector<vec2> &tmp_tex,
								 vector<vec3> &tmp_nor,
								 vector<uvec3> &pos_idxs,
								 vector<uvec3> &tex_idxs,
								 vector<uvec3> &nor_idxs);

	void produce_arrays_per_vertex(vector<vec3> &tmp_pos,
								   vector<vec2> &tmp_tex,
								   vector<vec3> &tmp_nor,
								   vector<uvec3> &pos_idxs,
								   vector<uvec3> &tex_idxs,
								   vector<uvec3> &nor_idxs);

	void calculate_per_vertex_normals();
	void calculate_per_face_normals();

	void prepare_vertices_and_indices();

	vector<vec3> pos;
	vector<vec2> tex;
	vector<vec3> nor;
	vector<uvec3> pos_idxs;


};

class IndexedModelObj : public IndexedModelFile
{
public:
	IndexedModelObj() {}
	~IndexedModelObj() {}

	IndexedModelObj(string filename, NormalIndexing mode);
	virtual void load(string filename, NormalIndexing mode) override;
};


#endif /* RENDERER/INDEXEDMODEL_H_ */
