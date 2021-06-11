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
#include "Math.h"

using namespace std;

class SceneSerializer;


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
	friend class TimePanel;
public:
    Scene();
	~Scene();

	Entity create_entity(string name = "Unnamed");
	Entity get_entity_by_uuid(unsigned int uuid);

	void draw(Entity&);
	void draw_depth_first(Entity&);

	void on_resize(float, float);
	void on_update_runtime(double);
	void on_update_shadow();
	void on_update_editor(double, EditorCamera&);

	void controllers_events(Event&);

	unsigned int id;
	static unsigned int num_scenes;
	static int scene_id(int entity_framebuffer_id);

	entt::registry *get_registry() { return &registry; }

	char flags = 0;

	Material scene_material;
	Entity root_entity;

	Entity observer, light;
	Entity target;

	// global variable :( to modify the behaviour of draw func
	bool check_for_shadow_casting = false;

private:
	entt::registry registry;
	static const unsigned int id_factor = 1000000;
};



#endif /* SCENE_H_ */
