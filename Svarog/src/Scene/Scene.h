#ifndef SCENE_H_
#define SCENE_H_

#include <algorithm>
#include <iostream>
#include <stdio.h>
// #include "Mesh.h"
// #include "VertexArrayObject.h"
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


class Scene
{
	friend class Entity;
public:
    Scene();
	~Scene();

	Entity create_entity(string name = "Default entity");

	void draw(Entity*);
	void draw_depth_first(Entity*);

// 	void on_update();
	void on_resize(int, int);
	void draw_root();

	Light* get_active_light();
	Transform* get_active_drawable_transform();

	Material scene_material;
	Renderer renderer;
	Entity root_entity;
	Camera *camera;
private:
	Entity *active_entity;
// 	Entity* get_active_depth_first(Entity*);

	entt::registry registry;
};



#endif /* SCENE_H_ */
