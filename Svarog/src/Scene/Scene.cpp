#include "svpch.h"
#include "Scene.h"


Scene::Scene()
{
	root_entity = create_entity("Root");
// 	observer = create_entity("Observer");
}


Scene::~Scene()
{
	if (framebuffer != NULL)
		delete framebuffer;

	auto view = registry.view<MeshComponent>();
	for (auto &e: view)
	{
		delete registry.get<MeshComponent>(e).vao;
	}
}

Entity Scene::create_entity(string name)
{
	Entity entity(registry.create(), &registry);
	entity.add_component<Transform>(Transform());
	entity.add_component<TagComponent>(name);

	return entity;
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
		Camera *cam = registry.get<CameraComponent>(e).camera;
		cam->aspect = width / (float)height;
	}

	if (flags & RENDER_TO_FRAMEBUFFER)
	{
        ASSERT(ms_framebuffer != NULL, "Multisampling Framebuffer is NULL");
        ASSERT(framebuffer != NULL, "Framebuffer is NULL");
        ms_framebuffer->resize(width, height);
        framebuffer->resize(width, height);
		Renderer::bind_default_framebuffer();
	}

}

void Scene::enable_render_to_framebuffer()
{
    flags |= RENDER_TO_FRAMEBUFFER;

    auto window = Application::get_window();

    ms_framebuffer = new Framebuffer(window->width, window->height,
                       COLOR_ATTACHMENT | DEPTH_ATTACHMENT | MULTISAMPLING);

	framebuffer = new Framebuffer(window->width, window->height,
                       COLOR_ATTACHMENT | DEPTH_ATTACHMENT);
}





void Scene::draw(Entity *entity)
{
	// is it used for anything ?
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


void Scene::draw_root()
{
    CORE_ASSERT(observer.has_component<CameraComponent>(),
                "Observer has no Camera Component");


    Renderer::set_viewport(0, 0, Application::get_window()->width,
                           Application::get_window()->height);

	if (flags & RENDER_TO_FRAMEBUFFER)
	{
		CORE_ASSERT(framebuffer != NULL, "Framebuffer is NULL");

		ms_framebuffer->bind();
		ms_framebuffer->clear();
	}
	else
    	Renderer::bind_default_framebuffer();

    draw_depth_first(&root_entity);


	if (flags & RENDER_TO_FRAMEBUFFER)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_framebuffer->id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->id);
		glBlitFramebuffer(0, 0,
				framebuffer->specification.width,
				framebuffer->specification.height,
				0, 0,
				framebuffer->specification.width,
				framebuffer->specification.height,
				GL_COLOR_BUFFER_BIT, GL_NEAREST);
		Renderer::bind_default_framebuffer();
	}
}

void Scene::on_update_runtime(double time_delta)
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

//     Transform &sct = observer.get_component<Transform>();
	Camera *cam = observer.get_component<CameraComponent>().camera;
    scene_material.uniforms_mat4["u_view_matrix"] = cam->get_view();
    scene_material.uniforms_mat4["u_perspective_matrix"] =
        cam->get_perspective();

	draw_root();
}


void Scene::on_update_editor(double time_delta, EditorCamera &editor_camera)
{
    scene_material.uniforms_mat4["u_view_matrix"] = editor_camera.get_view();
    scene_material.uniforms_mat4["u_perspective_matrix"] =
        editor_camera.get_perspective();
    draw_root();
}
