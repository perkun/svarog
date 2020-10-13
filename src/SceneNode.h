#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <stdio.h>
#include <iostream>
#include <vector>

#include "Mesh.h"
#include "Transform.h"

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

	virtual void update();
	virtual void draw();

	vector<SceneNode*> children;
private:
	Transform world_transform, transform;
	Mesh *mesh;

	SceneNode *parent;
};

#endif
