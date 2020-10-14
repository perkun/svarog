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
	SceneNode(Transform transform, Mesh *mesh);
	~SceneNode();
	void add_child(SceneNode *c);
// 	SceneNode* detatch_child(SceneNode*);
	void detatch();
	void bind_shader(Shader *s);

	virtual void update();
	virtual void draw();

	vector<SceneNode*> children;
private:
	SceneNode *parent;

	Transform world_transform, transform;
	Mesh *mesh;
	Shader *shader;
};

#endif
