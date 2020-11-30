#include "Scene.h"


Scene::Scene()
{
	root_entity = create_entity("Root");
// 	observer = create_entity("Observer");
}


Scene::~Scene()
{
	if (light.has_component<FramebufferComponent>())
		delete light.get_component<FramebufferComponent>().framebuffer;

	if (observer.has_component<FramebufferComponent>())
		delete observer.get_component<FramebufferComponent>().framebuffer;

	if (light.has_component<CameraComponent>())
		delete light.get_component<CameraComponent>().camera;

	if (observer.has_component<CameraComponent>())
		delete observer.get_component<CameraComponent>().camera;

	if (framebuffer != NULL)
		delete framebuffer;

}

Entity Scene::create_entity(string name)
{
	Entity entity(registry.create(), &registry);
	entity.add_component<Transform>(Transform());
	entity.add_component<TagComponent>(name);
	return entity;
}


void Scene::on_resize(float width, float height)
{

	auto view = registry.view<CameraComponent>();
	for (auto &e: view)
	{
		Camera *cam = registry.get<CameraComponent>(e).camera;
		cam->aspect = width / (float)height;
		cam->update();
	}
}





void Scene::draw(Entity *entity)
{
    if (entity->has_component<SceneStatus>())
    {
        SceneStatus &scene_status = entity->get_component<SceneStatus>();
        if (!scene_status.render)
            return;
    }

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

    if (entity->has_component<Material>())
    {
		Material &material = entity->get_component<Material>();
		material.uniforms_mat4["u_model_matrix"] = tr.world;

		if (entity->has_component<TextureComponent>())
			entity->get_component<TextureComponent>().texture->bind();

		material.set_uniforms();  // binds and sets

		scene_material.set_uniforms(material.shader);


        if (entity->has_component<MeshComponent>())
        {
            MeshComponent &mesh = entity->get_component<MeshComponent>();
            Renderer::draw(mesh.vao, material.shader);
        }
    }
}

void Scene::draw_depth_first(Entity *entity)
{
	draw(entity);
	for (Entity *child: entity->children)
		draw_depth_first(child);
}


void Scene::draw_root(POV perspective, double time_delta)
{

    // scene materials from observer:
	ASSERT(observer.has_component<Transform>())
	ASSERT(light.has_component<Transform>())

    Transform &sct = observer.get_component<Transform>();
    Transform &slt = light.get_component<Transform>();

    scene_material.uniforms_vec3["u_light_position"] = slt.position;
    scene_material.uniforms_mat4["u_light_view_matrix"] = slt.get_view();
    scene_material.uniforms_mat4["u_light_perspective_matrix"] =
        light.get_component<CameraComponent>().camera->get_perspective();


	ASSERT(light.has_component<FramebufferComponent>())
    Framebuffer *shadow_fb =
        light.get_component<FramebufferComponent>().framebuffer;

    if (perspective == POV::LIGHT)
    {
        shadow_fb->bind();
        shadow_fb->clear();

        scene_material.uniforms_mat4["u_view_matrix"] = slt.get_view();
        scene_material.uniforms_mat4["u_perspective_matrix"] =
            light.get_component<CameraComponent>().camera->get_perspective();

        draw_depth_first(&root_entity);

        shadow_fb->unbind();
    }
    else if (perspective == POV::OBSERVER)
    {
        scene_material.uniforms_mat4["u_view_matrix"] = sct.get_view();
        scene_material.uniforms_mat4["u_perspective_matrix"] =
            observer.get_component<CameraComponent>().camera->get_perspective();

		Renderer::set_viewport(0, 0, Application::get_window()->width,
                   			   Application::get_window()->height);

		if (render_to_framebuffer)
		{
			ASSERT(framebuffer != NULL)
			framebuffer->bind();
			framebuffer->clear();
		}
		else
			Renderer::bind_default_framebuffer();

        shadow_fb->bind_depth_texture(1);
		framebuffer->bind_color_texture(2);

        draw_depth_first(&root_entity);

		if (render_to_framebuffer)
			framebuffer->unbind();
    }
}

void Scene::on_update(double time_delta)
{
	//update scripts
	registry.view<NativeScriptComponent>().each([=](auto entity, auto&nsc)
	{
		if (!nsc.instance)
		{   // instanciate script
			nsc.instance = nsc.instantiate_script();
			nsc.instance->entity = Entity(entity, &(this->registry));
			nsc.instance->on_create();
		}
		nsc.instance->on_update(time_delta);

	});

    // From Light Perspective ////////
    draw_root(POV::LIGHT, time_delta);

    // From normal Camera /////////
    draw_root(POV::OBSERVER, time_delta);

}
