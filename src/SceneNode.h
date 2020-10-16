#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <stdio.h>
#include <iostream>
#include <vector>

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
	virtual void draw();
	void update_depth_first();
	void draw_depth_first();

	vector<SceneNode*> children;
	Transform world_transform, transform;


	Mesh *mesh;
	Shader *shader;
	SceneNode *parent;
private:
};

#endif
