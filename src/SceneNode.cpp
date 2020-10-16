#include "SceneNode.h"


SceneNode::SceneNode()
{
	parent = NULL;
	mesh = NULL;
	shader = NULL;

	transform.model_matrix = mat4(1);
	world_transform.model_matrix = mat4(1);
};


SceneNode::SceneNode(Mesh *mesh)
{
	parent = NULL;
	this->mesh = mesh;
	shader = NULL;

	transform.model_matrix = mat4(1);
	world_transform.model_matrix = mat4(1);
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
	if (parent == NULL)  // root
	{
		transform.update_model_matrix();
		world_transform.model_matrix = transform.model_matrix;
	}
	else
	{
		transform.update_model_matrix();
		world_transform.model_matrix =
			parent->world_transform.model_matrix * transform.model_matrix;
	}
}

void SceneNode::draw()
{
	if (mesh != NULL)
	{
		shader->set_uniform_mat4f("model_matrix", world_transform.model_matrix);
		shader->bind();
		mesh->draw();
	}
}


void SceneNode::update_depth_first()
{
	for (SceneNode *child: this->children)
	{
		child->update();
		child->update_depth_first();
	}
}


void SceneNode::draw_depth_first()
{
	for (SceneNode *child: this->children)
	{
		child->draw();
		child->draw_depth_first();
	}
}


void SceneNode::bind_shader(Shader *s)
{
	shader = s;
}

