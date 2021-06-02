#include "svpch.h"
#include "MainLayer.h"
#include "Components.h"
#include "Framebuffer.h"
#include "CameraController.h"
#include "Batch.h"
// #include "ImGuizmo.h"
#include "Math.h"
// #include "ModelController.h"
// #include "AsteroidController.h"
#include "Renderer.h"
#include "Utils/FileDialog.h"
#include "Utils/File.h"
// #include "ObservatoryPanel.h"
#include "Utils/ObjHeader.h"
#include "Utils/Time.h"
#include "SceneSerializer.h"
#include <glm/gtc/type_ptr.hpp>


unsigned int Scene::num_scenes = 0;

MainLayer::MainLayer(Args args)
{
    this->args = args;
    TRACE("MainLayer constructed");
}

MainLayer::~MainLayer()
{

}


void MainLayer::on_attach()
{
	Renderer::set_line_width(3);
    auto window = Application::get_window();
	init_model_pos = vec3(0., 0., 0.);

    FramebufferSpecification ms_fb_specs;
    ms_fb_specs.attachments = {FramebufferTextureFormat::RGBA32F,
                               FramebufferTextureFormat::RED_INTEGER,
                               FramebufferTextureFormat::DEPTH32FSTENCIL8};
    ms_fb_specs.width = window->width;
    ms_fb_specs.height = window->height;
    ms_fb_specs.samples = 4;

    ms_framebuffer = new Framebuffer(ms_fb_specs);

    FramebufferSpecification fb_specs;
    fb_specs.attachments = {FramebufferTextureFormat::RGBA32F,
                            FramebufferTextureFormat::RED_INTEGER,
                            FramebufferTextureFormat::DEPTH32FSTENCIL8};
    fb_specs.width = window->width;
    fb_specs.height = window->height;
    fb_specs.samples = 1;

    framebuffer = new Framebuffer(fb_specs);


    Entity model = scene.create_entity("Model");

    model.add_component<MeshComponent>(ModelType::ICO_SPHERE);
    model.add_component<Material>("basic_shader")
        .uniforms_int["u_has_texture"] = 0;
    //     model.add_component<NativeScriptComponent>().bind<AsteroidController>();
    model.add_component<OrbitalComponent>();
	model.get_component<Transform>().position = init_model_pos;

	Entity e = scene.create_entity("xyz axes");
	e.add_component<Material>("line_shader");
	Batch axes;
	axes.push_back(
			IndexedLine(vec3(0.), vec3(2., 0., 0), vec4(1., 0., 0., 1.)));
	axes.push_back(
			IndexedLine(vec3(0.), vec3(0., 2., 0), vec4(0., 1., 0., 1.)));
	axes.push_back(
			IndexedLine(vec3(0.), vec3(0., 0., 2), vec4(0., 0., 1., 1.)));
	e.add_component<MeshComponent>(
			make_shared<VertexArrayObject>(axes.indexed_model));
	e.get_component<SceneStatus>().casting_shadow = false;
	model.add_child(e);


    Entity runtime_observer = scene.create_entity("Observer");
    CameraComponent &rocp = runtime_observer.add_component<CameraComponent>(
        make_shared<OrthograficCamera>(2.25, 1.0, 0.1, 10.));
    runtime_observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();

    rocp.camera->position = vec3(0., -5, 0.);
    rocp.camera->update_target(init_model_pos);

    Transform &rot = runtime_observer.get_component<Transform>();
    rot.position = rocp.camera->position;
    rot.speed = 8.;

    scene.root_entity.add_child(model);

	scene.observer = runtime_observer;
	scene.target = model;
}


void MainLayer::on_detach()
{
    delete ms_framebuffer;
    delete framebuffer;
}


void MainLayer::on_update(double ts)
{
    ms_framebuffer->bind();
    ms_framebuffer->clear();
    ms_framebuffer->clear_attachment(1, -1); // ent_id "bg" to -1

    scene.on_update_runtime(ts);

    Framebuffer::blit(ms_framebuffer, framebuffer, 0, 0);
    Framebuffer::blit(ms_framebuffer, framebuffer, 1, 1);

    Renderer::bind_default_framebuffer();
}


void MainLayer::on_imgui_render()
{
    ImGui::DockSpaceOverViewport();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
    ImGui::Begin("Scene");

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
    {
        viewport_panel_size.x = vps.x;
        viewport_panel_size.y = vps.y;

        scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
        ms_framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        Renderer::bind_default_framebuffer();
    }

    long int tex_id = framebuffer->get_color_attachment_id(0);
    ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.y), ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}


void MainLayer::on_event(Event &e)
{
    EventDispacher dispatcher(e);

    dispatcher.dispatch<KeyReleasedEvent>(
        bind(&MainLayer::on_key_released_event, this, placeholders::_1));

    dispatcher.dispatch<MouseMovedEvent>(
        bind(&MainLayer::on_mouse_moved_event, this, placeholders::_1));

    scene.controllers_events(e);
}


void MainLayer::on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();
	if (key_code == GLFW_KEY_Q)
		Application::stop();
}


void MainLayer::on_mouse_moved_event(MouseMovedEvent &event)
{
    vec2 cursor_shift = event.get_cursor_pos() - last_cursor_pos;

    if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_1))
    {
        // 		cout << cursor_shift.x << endl;
        Transform &t = scene.target.get_component<Transform>();
        //         t.rotation.z += cursor_shift.x / 400;
        //
        // //         mat4 mrot = rotate(cursor_shift.x / 400, vec3(0.,
        // 0., 1.));
        //         vec3 nx = vec3(glm::inverse(t.get_local_tansform()) * vec4(1,
        //         0, 0, 0));
        // //
        // // 		mrot = glm::inverse(t.get_local_tansform()) * mrot;
        //         mat4 mrot = rotate(cursor_shift.y / 400, nx);
        //
        //         vec3 trans, rot, scale;
        //         Math::decompose_transform(mrot, trans, rot, scale);
        //         //
        //         t.rotation += rot;


        mat4 mrot1 = rotate(cursor_shift.x / 400, vec3(0, 0, 1));
        mat4 mrot2 =
            rotate(cursor_shift.y / 400,
                   vec3(inverse(t.get_local_tansform()) * vec4(1, 0, 0, 0)));

        vec3 trans, rot, scale;
        Math::decompose_transform(mrot2 * mrot1, trans, rot, scale);
        t.rotation += rot;
    }

    last_cursor_pos = event.get_cursor_pos();
}
