#include "Scene.h"


Scene::Scene(Camera &c)  : active_camera(c)
{
	root_entity = create_entity("Root");
}


Scene::~Scene()
{
}

Entity Scene::create_entity(string name)
{
	Entity entity(registry.create(), &registry);
	entity.add_component<Transform>(Transform());
	entity.add_component<Tag>(Tag(name));
	return entity;
}


void Scene::draw(Entity &entity, Camera &camera)
{
	// update local transform, then multiply by parent.local
	Transform &tr = entity.get_component<Transform>();
	tr.update_local();
	if (entity.parent == NULL)
		tr.world = tr.local;
	else
	{
		Transform &pt = entity.parent->get_component<Transform>();
		tr.world = tr.local * pt.world;
	}

	if (entity.has_component<Mesh>() && entity.has_component<Shader>())
	{
		Shader &shader = entity.get_component<Shader>();

		shader.set_uniform_mat4f("model_matrix", tr.world);
		shader.set_uniform_mat4f("view_matrix", camera.get_view());
		shader.set_uniform_mat4f("perspective_matrix", camera.get_perspective());
// 		shader.set_uniform_4fv("u_color", vec4(1.0));

		if (entity.has_component<Texture>())
		{
			Texture &t = entity.get_component<Texture>();
			t.bind();
			shader.set_uniform_1i("u_texture", 0);
		}

		Mesh &mesh = entity.get_component<Mesh>();
		renderer.draw(mesh, shader);
	}
}


void Scene::draw_depth_first(Entity &entity, Camera &camera)
{
	draw(entity, camera);
	for (Entity *child: entity.children)
		draw_depth_first(*child, camera);
}


void Scene::draw_root()
{
	draw_depth_first(root_entity, active_camera);
}


