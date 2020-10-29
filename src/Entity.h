#ifndef ENTITY_H_
#define ENTITY_H_

#include <iostream>
#include <stdio.h>
#include "Scene.h"
#include "vendor/entt/entt.hpp"

using namespace std;

class Entity
{
public:
    Entity(entt::entity handle, Scene* scene);
	~Entity();

	template<typename T>
	bool has_component() {
		return scene->registry.has<T>(entity_handle);
	}

private:
	entt::entity entity_handle;
	Scene *scene = NULL;
};

#endif /* ENTITY_H_ */
