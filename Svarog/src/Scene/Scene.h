#ifndef SCENE_H_
#define SCENE_H_

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include "Core.h"
#include "Application.h"
#include "Components.h"
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Light.h"
#include "vendor/entt/entt.hpp"
#include "Entity.h"
#include "Material.h"

using namespace std;


enum class POV
{
	OBSERVER, LIGHT
};

class Scene
{
	friend class Entity;
public:
    Scene();
	~Scene();

	Entity create_entity(string name = "Default entity");

	void draw(Entity*);
	void draw_depth_first(Entity*);

	void on_resize(int, int);
	void draw_root(POV);

	Transform* get_active_drawable_transform();

	Material scene_material;
	Renderer renderer;
	Entity root_entity;

	Entity observer;
	Entity light;
private:
	Entity *active_entity;
// 	Entity* get_active_depth_first(Entity*);

	entt::registry registry;
};



#endif /* SCENE_H_ */
