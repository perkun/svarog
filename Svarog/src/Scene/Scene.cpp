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
//

// void Scene::set_active_entity(Entity *entity)
// {
// 	active_entity = entity;
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

Camera *Scene::get_active_camera()
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

Light* Scene::get_active_light()
{
	auto view = registry.view<Light, SceneStatus>();
	for (auto e: view)
	{
// 		if (registry.get<SceneStatus>(e).active)
// 		{
			return &registry.get<Light>(e);
// 		}
	}
	return NULL;
}


void Scene::draw(Entity *entity)
{
    if (entity->has_component<SceneStatus>())
    {
        SceneStatus &scene_status = entity->get_component<SceneStatus>();
        if (!scene_status.render)
            return;
    }

    Camera *camera = get_active_camera();

    // update local transform, then multiply by parent.local
    Transform &tr = entity->get_component<Transform>();
    tr.update_local();

    if (entity->parent == NULL)
        tr.world = tr.local;
    else
    {
        Transform &pt = entity->parent->get_component<Transform>();
        tr.world = pt.world * tr.local;
    }

    if (entity->has_component<Shader>())
    {
        Shader &shader = entity->get_component<Shader>();

        if (entity->has_component<Material>())
            shader.set_uniforms(entity->get_component<Material>()); // binds shader

		shader.set_uniform_mat4f("u_model_matrix", tr.world);  // not binding

		shader.set_uniforms(scene_material);


        if (entity->has_component<Mesh>())
        {
            Mesh &mesh = entity->get_component<Mesh>();

//             shader.set_uniform_mat4f("view_matrix", camera->get_view());
//             shader.set_uniform_mat4f("perspective_matrix",
//                                      camera->get_perspective());



            if (entity->has_component<Texture>())
            {
                Texture &t = entity->get_component<Texture>();
                t.bind();
                shader.set_uniform_1i("u_texture", 0);
            }

            renderer.draw(mesh, shader);
        }
    }
}

void Scene::draw_depth_first(Entity *entity)
{
// 	Transform &t = entity.get_component<Transform>();
// 	cout << t.position.x << "\n";

	draw(entity);
	for (Entity *child: entity->children)
		draw_depth_first(child);
}


void Scene::draw_root()
{
    Camera *camera = get_active_camera();
    if (camera == NULL)
    {
        cout << "No active Camera set, not rendering" << endl;
        return;
    }

// 	auto view = registry.view<Light, Mesh>();
// 	for (auto e: view)
// 	{
// 		Transform &tr = registry.get<Transform>(e);
// 		vec3 dir = normalize(registry.get<Light>(e).direction);
//
// 		tr.beta = acos(dir.z);
// 		tr.alpha = atan2(dir.y, dir.x) + M_PI/2.;
// 	}


    // update scene material
    scene_material.uniforms_mat4["u_view_matrix"] = camera->get_view();
    scene_material.uniforms_mat4["u_perspective_matrix"] =
        camera->get_perspective();

// 	if (get_active_light() != NULL)
// 		scene_material.uniforms_vec3["u_light_direction"] =
// 			get_active_light()->direction;
// 	else {
// 		cout << "No light in the scene" << endl;
// 	}

    // 	Transform &t = root_entity.get_component<Transform>();
    // 	cout << t.position.x << "\n";
    draw_depth_first(&root_entity);
}
