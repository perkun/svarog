#include "svpch.h"
#include "Entity.h"
#include "Components.h"




Entity::~Entity()
{
}


void Entity::add_child(Entity &child)
{
	child.get_component<SceneGraphComponent>().parent = *this;
	get_component<SceneGraphComponent>().children.push_back(child);
}


void Entity::detatch()
{
    Entity &parent = get_component<SceneGraphComponent>().parent;

    if (!parent) // no parent
        return;

	vector<Entity> &parent_children =
		parent.get_component<SceneGraphComponent>().children;
    for (unsigned int i = 0; i < parent_children.size(); i++)
        if (parent_children[i] == *this)
            parent_children.erase(parent_children.begin() + i);

    parent = Entity();
}
