#ifndef INDEXEDMODEL_H_
#define INDEXEDMODEL_H_

#include "Core.h"
#include "VertexLayout.h"


using namespace std;
using namespace glm;

enum class NormalIndexing
{
	PER_VERTEX,
	PER_FACE
};

enum class ModelType
{
	ICO_SPHERE,
	CUBE,
	QUAD,
	FILE,
	LINE
};

class IndexedModel
{
public:
	IndexedModel() {}
	~IndexedModel() {}

	VertexLayout layout;
	vector<float> vertices;
	vector<unsigned int> indices;

	unsigned int draw_type = GL_TRIANGLES;
	float get_r_max() const;
};

class IndexedIcoSphere : public IndexedModel
{
public:
	IndexedIcoSphere();
	IndexedIcoSphere(vec3 position, vec3 scale);
	~IndexedIcoSphere() {}
protected:
	void create(vec3, vec3);
};


class IndexedLine : public IndexedModel
{
public:
	IndexedLine(vec3 start, vec3 stop, vec4 color);

};


class IndexedQuad : public IndexedModel
{
public:
	IndexedQuad();
	IndexedQuad(vec3 position, vec2 scale);
	~IndexedQuad() {}
protected:
	void create(vec3 position, vec2 scale);
};


class IndexedCube : public IndexedModel
{
public:
	IndexedCube();
	IndexedCube(vec3 position, vec3 scale);
	~IndexedCube() {}

protected:
	void create(vec3 position, vec3 scale);
};


class IndexedColorCube : public IndexedModel
{
public:
	IndexedColorCube();
	IndexedColorCube(vec3 position, vec3 scale, vec4 color);
	~IndexedColorCube() {}

protected:
	void create(vec3 position, vec3 scale, vec4 color);
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
	void calculate_texture_coords();

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


class IndexedModelShp : public IndexedModelFile
{
public:
	IndexedModelShp() {}
	~IndexedModelShp() {}

	IndexedModelShp(string filename, NormalIndexing mode);
	virtual void load(string filename, NormalIndexing mode) override;
};


#endif /* RENDERER/INDEXEDMODEL_H_ */
