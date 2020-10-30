#include "Scene.h"


Scene::Scene()
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


void Scene::on_resize(int width, int height)
{
	auto view = registry.view<Camera>();
	for (auto e: view)
	{
		Camera &camera = registry.get<Camera>(e);
		camera.aspect = width / (float)height;
		camera.update();
	}

}


// Entity* Scene::get_active_depth_first(Entity *entity)
// {
// 	Entity *tmp = NULL;
//
// 	if (entity->active)
// 		tmp = entity;
// 	else
// 		for (Entity *child: entity->children)
// 			tmp = get_active_depth_first(child);
//
// 	return tmp;
// }

// Entity* Scene::get_active_entity()
// {
// 	// TODO: search for active only when updated
// 	active_entity = get_active_depth_first(&root_entity);
// 	return active_entity;
// }

Transform* Scene::get_active_drawable_transform()
{
	auto view = registry.view<Mesh, SceneStatus>();
	for (auto e: view)
	{
		SceneStatus &ss = registry.get<SceneStatus>(e);
		if (ss.active)
		{
			return &registry.get<Transform>(e);

		}
	}
	return NULL;
}


Camera* Scene::get_active_camera()
{
	auto view = registry.view<Camera, SceneStatus>();
	for (auto e: view)
	{
		if (registry.get<SceneStatus>(e).active)
		{
			return &registry.get<Camera>(e);
		}
	}
	return NULL;
}


void Scene::draw(Entity &entity)
{
	Camera camera;
	auto view = registry.view<Camera, SceneStatus>();
	for (auto e: view)
	{
		if (registry.get<SceneStatus>(e).active)
		{
			camera = registry.get<Camera>(e);
			break;
		}
	}


	// update local transform, then multiply by parent.local
	Transform &tr = entity.get_component<Transform>();
	tr.update_local();
	if (entity.parent == NULL)
		tr.world = tr.local;
	else
	{
		Transform &pt = entity.parent->get_component<Transform>();
		tr.world = pt.world * tr.local;
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


void Scene::draw_depth_first(Entity &entity)
{
	draw(entity);
	for (Entity *child: entity.children)
		draw_depth_first(*child);
}


void Scene::draw_root()
{
	draw_depth_first(root_entity);
}


