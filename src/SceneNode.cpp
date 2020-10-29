#include "SceneNode.h"


SceneNode::SceneNode()
{
	transform.model_matrix = mat4(1);
	world_transform.model_matrix = mat4(1);
};


SceneNode::SceneNode(Mesh *mesh)
{
	this->mesh = mesh;

	transform.model_matrix = mat4(1);
	world_transform.model_matrix = mat4(1);
};


SceneNode::SceneNode(Transform transform, Mesh *mesh = NULL)
{
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
		camera = parent->camera;
	}

}

void SceneNode::draw(Renderer &renderer)
{

	// UPDATE
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
		camera = parent->camera;
	}

	if (mesh != NULL)
	{
		if (texture != NULL)
		{
			texture->bind();
			shader->set_uniform_1i("u_texture", 0);
		}
    	shader->set_uniform_4fv("u_color", color);
		shader->set_uniform_mat4f("view_matrix", camera->get_view());
		shader->set_uniform_mat4f("perspective_matrix", camera->get_perspective());
		shader->set_uniform_mat4f("model_matrix", world_transform.model_matrix);
	}

	renderer.draw(*mesh, *shader);
}




void SceneNode::update_depth_first()
{
	for (SceneNode *child: this->children)
	{
		child->update();
		child->update_depth_first();
	}
}


void SceneNode::draw_depth_first(Renderer &renderer)
{
	for (SceneNode *child: this->children)
	{
		child->draw(renderer);
		child->draw_depth_first(renderer);
	}
}



void SceneNode::bind_shader(Shader *s)
{
	shader = s;
}

void SceneNode::bind_texture(Texture *t)
{
	texture = t;
}

