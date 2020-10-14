#include "SceneNode.h"


SceneNode::SceneNode()
{
	parent = NULL;
	mesh = NULL;
	shader = NULL;
};


SceneNode::SceneNode(Transform transform, Mesh *mesh = NULL)
{
	parent = NULL;
	this->transform = transform;
	this->mesh = mesh;
};


SceneNode::~SceneNode()
{
	for (SceneNode *c: children)
		delete c;
}

void SceneNode::add_child(SceneNode *c)
{
	c->parent = this;
	children.push_back(c);
}

// SceneNode* SceneNode::detatch_child(SceneNode* n)
// {
// 	for (uint i = 0; i < children.size(); i++)
// 		if (children[i] == n)
// 			children.erase(children.begin() + i);
// 	return n;
//
// }


void SceneNode::detatch()
{
	if (parent == NULL)
		return;

	for (uint i = 0; i < parent->children.size(); i++)
		if (parent->children[i] == this)
			parent->children.erase(parent->children.begin() + i);

	parent = NULL;
}




void SceneNode::update()
{
	return;
}

void SceneNode::draw()
{
	if (mesh != NULL)
	{
		shader->bind();
		mesh->draw();
	}
}


void SceneNode::bind_shader(Shader *s)
{
	shader = s;
}

