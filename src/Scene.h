#ifndef SCENE_H_
#define SCENE_H_

#include <iostream>
#include <stdio.h>
#include "vendor/entt/entt.hpp"

using namespace std;

class Entity;

class Scene
{
	friend class Enitty;
public:
    Scene();
	~Scene();

	Entity create_entity();

	void on_update();
private:
	entt::registry registry;


};

#endif /* SCENE_H_ */
