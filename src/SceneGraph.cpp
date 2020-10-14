#include "SceneGraph.h"

SceneGraph::SceneGraph()
{
	root = new SceneNode;
}


// use function pointer to do whatever with a node
void SceneGraph::depth_first_search(SceneNode *root, void (*func)(SceneNode* n))
{
	for (SceneNode *child: root->children)
	{
		func(child);
		depth_first_search(child, func);
	}
}

void SceneGraph::draw_depth_first(SceneNode *r)
{
	for (SceneNode *child: r->children)
	{
		child->draw();
		draw_depth_first(child);
	}

}
