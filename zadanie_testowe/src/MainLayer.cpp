#include "svpch.h"
#include "MainLayer.h"
#include "Components.h"
#include "Framebuffer.h"
#include "Math.h"
#include "Renderer.h"
#include "Utils/FileDialog.h"
#include "Utils/ObjHeader.h"


unsigned int Scene::num_scenes = 0;

MainLayer::MainLayer()
{
}

MainLayer::~MainLayer()
{
}


void MainLayer::on_attach()
{
    Renderer::set_line_width(2);
    auto window = Application::get_window();


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


    Entity runtime_observer = scene.create_entity("Observer");
    CameraComponent &rocp = runtime_observer.add_component<CameraComponent>(
        make_shared<OrthograficCamera>(cam_size_x, 1.0, 0.1, 10.));

    rocp.camera->position = vec3(0., 0., 5.);
    rocp.camera->update_target(vec3(0, 0, 0));
	rocp.camera->up = vec3(0,1,0);

    Transform &rot = runtime_observer.get_component<Transform>();
    rot.position = rocp.camera->position;

    scene.observer = runtime_observer;

    new_line_ent = scene.create_entity("new line");
    new_line_ent.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedLine(vec3(0.),
				vec3(-0.2, 0.2, 0.0), vec4(0.2, 0.8, 0.2, 1.0))));
    new_line_ent.add_component<Material>("line_shader");


    scene.root_entity.add_child(new_line_ent);
    scene.target = new_line_ent;
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

	scene.update_scripts(ts);
    scene.on_update_runtime(ts);

    Framebuffer::blit(ms_framebuffer, framebuffer, 0, 0);
    Framebuffer::blit(ms_framebuffer, framebuffer, 1, 1);

    Renderer::bind_default_framebuffer();
}


void MainLayer::on_imgui_render()
{

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
    //
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoDecoration;


    ImGui::Begin("Scene", NULL, window_flags);


    ImVec2 vps = ImGui::GetContentRegionAvail();
    if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
    {
        viewport_panel_size.x = vps.x;
        viewport_panel_size.y = vps.y;

        auto cam = dynamic_pointer_cast<OrthograficCamera>(
            scene.observer.get_component<CameraComponent>().camera);
        float aspect = viewport_panel_size.x / viewport_panel_size.y;
        if (aspect > 1.) // wide screen
            cam->fov = cam_size_x * aspect;

        scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
        ms_framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        Renderer::bind_default_framebuffer();
    }

    long int tex_id = framebuffer->get_color_attachment_id(0);
    ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.y), ImVec2(0, 1),
                 ImVec2(1, 0));

    if (ImGui::BeginPopupContextItem("item context menu"))
    {
		context_menu_open = true;
		float value;
        if (ImGui::Selectable("line"))
		{
            value = 0.0f;
			context_menu_open = false;
		}
        if (ImGui::Selectable("curve"))
		{
            value = 0.0f;
			context_menu_open = false;
		}


        ImGui::SetNextItemWidth(-1);
        ImGui::EndPopup();
    }

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}


void MainLayer::on_event(Event &e)
{
    EventDispacher dispatcher(e);

    dispatcher.dispatch<KeyReleasedEvent>(
        bind(&MainLayer::on_key_released_event, this, placeholders::_1));

    dispatcher.dispatch<KeyPressedEvent>(
        bind(&MainLayer::on_key_pressed_event, this, placeholders::_1));

    dispatcher.dispatch<MouseMovedEvent>(
        bind(&MainLayer::on_mouse_moved_event, this, placeholders::_1));

    dispatcher.dispatch<MouseButtonPressedEvent>(bind(
        &MainLayer::on_mouse_button_pressed_event, this, placeholders::_1));
}


void MainLayer::on_key_pressed_event(KeyPressedEvent &event)
{
}

void MainLayer::on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();
	if (key_code == GLFW_KEY_Q)
		Application::stop();
}


vec2 MainLayer::get_mouse_pos()
{
    vec2 mp = Input::get_mouse_position();
    int win_w = Application::get_window()->width;
    int win_h = Application::get_window()->height;

    float aspect = viewport_panel_size.x / viewport_panel_size.y;
    vec2 mouse_pos;

    if (aspect > 1.)
    {
        mouse_pos.x = (mp.x / win_w - 0.5) * aspect;
        mouse_pos.y = 1. - mp.y / win_h - 0.5;
    }
    else
    {
        mouse_pos.x = (mp.x / win_w - 0.5);
        mouse_pos.y = (1. - mp.y / win_h - 0.5) / aspect;
    }

	return mouse_pos;
}


void MainLayer::on_mouse_moved_event(MouseMovedEvent &event)
{
	vec3 start, stop;

	if (lines.size() > 0)
    {
        start = lines[lines.size() - 1].stop;
    }
	else
		start = vec3(0.0);

	stop = vec3(get_mouse_pos(), 0.);

	new_line = Line(start, stop, vec4(0.2, 0.8, 0.2, 1.0));
	new_line_ent.remove_component<MeshComponent>();
    new_line_ent.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedLine(
				start,
				stop,
				vec4(0.2, 0.8, 0.2, 1.0))));
}

void MainLayer::on_mouse_button_pressed_event(MouseButtonPressedEvent &e)
{
	if (e.get_button_code() != GLFW_MOUSE_BUTTON_1)
		return;

	if (context_menu_open)
		return;



    lines.push_back(Line(new_line.start, new_line.stop));

    Batch lines_batch;
    for (Line line : lines)
        lines_batch.push_back(line.indexed_model);

    if (lines_ent)
    {
		TRACE("Geting vao. lines size: {}", lines.size());

		lines_ent.remove_component<MeshComponent>();
        lines_ent.add_component<MeshComponent>(
            make_shared<VertexArrayObject>(lines_batch.indexed_model));
    }
    else
    {
        lines_ent = scene.create_entity("all lines");
        lines_ent.add_component<Material>("line_shader");
        lines_ent.add_component<MeshComponent>(
            make_shared<VertexArrayObject>(lines_batch.indexed_model));
        scene.root_entity.add_child(lines_ent);


        auto vao = static_pointer_cast<DynamicVertexArrayObject>(
            lines_ent.get_component<MeshComponent>().vao);
        vao->update_buffer(lines_batch.indexed_model);
// 		TRACE("vao create: {}", vao->vao_id);
    }
}
