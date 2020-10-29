#ifndef SCENE_H_
#define SCENE_H_

#include <iostream>
#include <stdio.h>
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "vendor/entt/entt.hpp"

using namespace std;

class Entity;

class Scene
{
	friend class Entity;
public:
    Scene();
	~Scene();

	Entity create_entity(string name = "Default entity");

	void draw(Entity*, Camera*);
	void draw_depth_first(Entity*, Camera*);

	void on_update();

	Renderer renderer;
private:
	entt::registry registry;
};



#endif /* SCENE_H_ */
