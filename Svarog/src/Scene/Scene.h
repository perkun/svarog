#ifndef SCENE_H_
#define SCENE_H_

#include "Application.h"
#include "Components.h"
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "entt.hpp"
#include "Entity.h"
#include "Material.h"
#include "EditorCamera.h"

using namespace std;

class SceneSerializer;

enum class POV
{
	OBSERVER, LIGHT
};


enum  SceneFlags {
	RENDER_TO_FRAMEBUFFER = 0b1,
	SHADOW_MAP = 0b10
};



class Scene
{
	friend class Entity;
	friend class Layer;
	friend class SceneLayer;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
public:
    Scene();
	~Scene();

	Entity create_entity(string name = "Unnamed");

	void draw(Entity&);
	void draw_depth_first(Entity&);

	void on_resize(float, float);
	void on_update_runtime(double);
	void on_update_editor(double, EditorCamera&);

	void controllers_events(Event&);

	char flags = 0;

	Material scene_material;
	Entity root_entity;

	Entity observer;
	Entity light;

	Entity selected_entity;

	// TODO: create assets ledger in Application
	map<string, Shader*> shaders;

private:
	entt::registry registry;
};



#endif /* SCENE_H_ */
