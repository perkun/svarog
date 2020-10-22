#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"

using namespace std;


class SceneNode
{
public:
	SceneNode();
	SceneNode(Mesh *mesh);
	SceneNode(Transform transform, Mesh *mesh);
	~SceneNode();

	void add_child(SceneNode *c);
// 	SceneNode* detatch_child(SceneNode*);
	void detatch();
	void bind_shader(Shader *s);

	virtual void update();
	virtual void draw(mat4, mat4);
	void update_depth_first();
	void draw_depth_first(mat4, mat4);

	vector<SceneNode*> children;
	Transform world_transform, transform;


	Mesh *mesh;
	Shader *shader;
	SceneNode *parent;
private:
};

#endif
