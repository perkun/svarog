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
	if (!*this)
		return;

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

Entity& Entity::get_parent()
{
	return get_component<SceneGraphComponent>().parent;
}

vector<Entity>& Entity::get_children()
{
	return get_component<SceneGraphComponent>().children;
}

void Entity::destroy()
{
	if (!*this)
		return;

	vector<Entity>& children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		children[i].destroy();
	}

	string tag = get_component<TagComponent>().tag;

	detatch();
	scene_registry_handle->destroy(entity_handle);
	// 	givs segfault if exiting program with non-empty observation series...
	// 	it is ok on deleting observations though.
// 	WARN("Deleted Entity {}", tag);
}
