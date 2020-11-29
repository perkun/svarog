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

class SceneSerializer;

enum class POV
{
	OBSERVER, LIGHT
};

class Scene
{
	friend class Entity;
	friend class SceneLayer;
	friend class SceneSerializer;
public:
    Scene();
	~Scene();

	Entity create_entity(string name = "Default entity");

	void draw(Entity*);
	void draw_depth_first(Entity*);

	void on_resize(float, float);
	void on_update(double);

	bool render_to_framebuffer = false;
	Framebuffer *framebuffer = NULL;

	Material scene_material;
	Entity root_entity;

	Entity observer;
	Entity light;
private:
	void draw_root(POV, double);  // equivalent of on_update()
	entt::registry registry;
};



#endif /* SCENE_H_ */
