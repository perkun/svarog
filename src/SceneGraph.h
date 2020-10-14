#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include <iostream>
#include <stdio.h>
#include "SceneNode.h"

using namespace std;

class SceneGraph
{
public:
	SceneGraph();
	void depth_first_search(SceneNode *root, void (*func)(SceneNode* n));
// 	void update_all();
	void draw_depth_first(SceneNode*);

	SceneNode *root;
};

#endif /* TREE_H_ */
