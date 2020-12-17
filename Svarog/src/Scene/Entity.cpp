#include "svpch.h"
#include "Entity.h"
#include "Components.h"




Entity::~Entity()
{
}


void Entity::add_child(Entity &child)
{
	child.get_component<SceneGraphComponent>().parent = Entity(entity_handle,
		scene_registry_handle);
	get_component<SceneGraphComponent>().children.push_back(child);
}


void Entity::detatch()
{
//     if (!get_component<SceneGraphComponent>().parent)
//         return;
//
//     Entity &parent = get_component<SceneGraphComponent>().parent;
//     for (unsigned int i = 0;
//          i < parent.get_component<SceneGraphComponent>().children.size(); i++)
//         if (parent.get_component<SceneGraphComponent>().children[i] == this)
//             parent.get_component<SceneGraphComponent>().children.erase(
//                 parent.get_component<SceneGraphComponent>().children.begin() +
//                 i);
//
//     parent = Entity();
}
