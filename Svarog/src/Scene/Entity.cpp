#include "svpch.h"
#include "Entity.h"




Entity::~Entity()
{
}


void Entity::add_child(Entity *child)
{
	child->parent = this;
	children.push_back(child);
}


void Entity::detatch()
{
	if (parent == NULL)
		return;

	for (unsigned int i = 0; i < parent->children.size(); i++)
		if (parent->children[i] == this)
			parent->children.erase(parent->children.begin() + i);

	parent = NULL;
}

