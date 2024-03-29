#include "svpch.h"
#include "Scene.h"
#include "Renderer.h"


Scene::Scene()
{
	root_entity = create_entity("root");

	id = num_scenes;
	num_scenes++;
}


Scene::~Scene()
{
}

Entity Scene::create_entity(string name)
{
	Entity entity(registry.create(), &registry);
	entity.add_component<Transform>(Transform());
	entity.add_component<TagComponent>(name);
	entity.add_component<SceneGraphComponent>();
	entity.add_component<SceneStatus>();



	return entity;
}

Entity Scene::get_entity_by_uuid(unsigned int uuid)
{
	auto view = registry.view<SceneGraphComponent>();
	for (auto &e: view)
	{
		Entity entity(e, &registry);
		if (entity.get_uuid() == uuid)
			return entity;
	}
	return Entity();
}

int Scene::scene_id(int entity_framebuffer_id)
{
	return (int)((float)entity_framebuffer_id / id_factor);
}


void Scene::controllers_events(Event &event)
{
	/// send event to all the controllers
	registry.view<NativeScriptComponent>().each([&event](auto entity, auto&nsc)
	{
		if (!nsc.instance)
			return;

		nsc.instance->on_event(event);
	});
}


void Scene::on_resize(float width, float height)
{
	auto view = registry.view<CameraComponent>();
	for (auto &e: view)
	{
		shared_ptr<Camera> cam = registry.get<CameraComponent>(e).camera;
		cam->aspect = width / (float)height;
	}
}


void Scene::draw(Entity &entity)
{
	CORE_ASSERT(entity.has_component<Transform>(), "entity not valid");

	// is it used for anything ?
    if (entity.has_component<SceneStatus>())
    {
        SceneStatus &scene_status = entity.get_component<SceneStatus>();
        if (!scene_status.render)
            return;

		if (check_for_shadow_casting)
			if (!scene_status.casting_shadow)
				return;
    }

    // update local transform, then multiply by parent.local
    Transform &tr = entity.get_component<Transform>();
    tr.update_local();


    if (!entity.get_component<SceneGraphComponent>().parent)
        tr.world = tr.local;
    else
    {
        Transform &pt = entity.get_component<SceneGraphComponent>()
                            .parent.get_component<Transform>();
        tr.world = pt.world * tr.local;
    }

    if (entity.has_component<Material>())
    {
		Material &material = entity.get_component<Material>();
		material.uniforms_mat4["u_model_matrix"] = tr.world;
		material.uniforms_int["u_entity_id"] =
			entity.get_uuid() + (id * id_factor);

		if (entity.has_component<TextureComponent>())
			entity.get_component<TextureComponent>().texture->bind();

        if (entity.has_component<MeshComponent>())
        {
            MeshComponent &mesh = entity.get_component<MeshComponent>();
			Renderer::submit(mesh.vao, material);
        }
    }
}

void Scene::draw_depth_first(Entity &entity)
{
	draw(entity);
	for (Entity &child: entity.get_component<SceneGraphComponent>().children)
		draw_depth_first(child);
}

void Scene::update_scripts(double time_delta)
{
    registry.view<NativeScriptComponent>().each([=](auto entity, auto &nsc) {
        if (!nsc.instance)
        { // instanciate script
            nsc.instance = nsc.instantiate_script();
            nsc.instance->entity = Entity(entity, &(this->registry));
            nsc.instance->on_create();
        }
        nsc.instance->on_update(time_delta);
    });
}


void Scene::on_update_runtime(double time_delta)
{
    CORE_ASSERT(observer.has_component<CameraComponent>(),
                "Observer does not have a CameraComponent");

    Renderer::begin_scene(observer.get_component<CameraComponent>().camera);
    if (light)
    {
        Renderer::set_dir_light(light.get_component<CameraComponent>().camera);
    }

    draw_depth_first(root_entity);


    Renderer::end_scene();
}

void Scene::on_update_shadow()
{
    CORE_ASSERT(observer.has_component<CameraComponent>(),
                "Observer does not have a CameraComponent");

	check_for_shadow_casting = true;
    Renderer::begin_scene(observer.get_component<CameraComponent>().camera);
    if (light)
	{
        Renderer::set_dir_light(light.get_component<CameraComponent>().camera);
	}
    draw_depth_first(root_entity);
    Renderer::end_scene();
	check_for_shadow_casting = false;
}


void Scene::on_update_editor(double time_delta, EditorCamera &editor_camera)
{
	Renderer::begin_scene(editor_camera);
	if (light)
	{
		Renderer::set_dir_light(light.get_component<CameraComponent>().camera);
	}
    draw_depth_first(root_entity);
	Renderer::end_scene();
}
