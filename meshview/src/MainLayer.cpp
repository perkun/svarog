#include "svpch.h"
#include "MainLayer.h"
#include "Components.h"
#include "Framebuffer.h"
#include "CameraController.h"
#include "ModelController.h"
#include "Batch.h"
// #include "ImGuizmo.h"
#include "Math.h"
// #include "ModelController.h"
// #include "AsteroidController.h"
#include "Renderer.h"
#include "Utils/FileDialog.h"
// #include "ObservatoryPanel.h"
#include "Utils/ObjHeader.h"
#include "Image.h"
#include <glm/gtc/type_ptr.hpp>


unsigned int Scene::num_scenes = 0;

MainLayer::MainLayer(cppargs::Args args)
{
    this->args = args;
    TRACE("MainLayer constructed");

    bg_colors.push_back(
        Application::get_bg_color());          // defalult application color
    bg_colors.push_back(vec4(1.));             // white
    bg_colors.push_back(vec4(0., 0., 0., 1.)); // black

	help_overlay = Overlay(1, 0.7, "help_overlay");
    help_overlay.text = "        H - this help\n"
                        "        I - toggle header info\n"
                        "        A - toggle xyz axes\n"
                        "        Z - show model from from x axis\n"
                        "SHIFT + Z - show model from from -x axis\n"
                        "        Y - show model from from y axis\n"
                        "SHIFT + Y - show model from from -y axis\n"
                        "        Z - show model from from z axis\n"
                        "SHIFT + Z - show model from from -z axis\n"
                        "        B - toggle background color\n\n"
                        "        P - save print screen \n\n"
                        "MOUSE:\n"
                        " LEFT BTN - rotate \n"
                        "RIGHT BTN - move \n"
                        "   SCROLL - zoom in/out \n";

	header_overlay = Overlay(0, 0, "header overlay");
	info_overlay = Overlay(2, 0.7, "info overlay");
}

MainLayer::~MainLayer()
{

}


void MainLayer::on_attach()
{
    Renderer::set_line_width(5);
    auto window = Application::get_window();
    init_model_pos = vec3(0., 0., 0.);

	viewport_panel_size.x = window->width;
	viewport_panel_size.y = window->height;

    // TODO couple more useful shaders
    shader_ptrs.push_back(Application::shaders["basic_shader"]);
    shader_ptrs.push_back(Application::shaders["tex_sha_ls"]);

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


    model = scene.create_entity("Model");


    if (args["model"])
        model.add_component<MeshComponent>(
            args.get_positional<string>("model"));
    else
        model.add_component<MeshComponent>(ModelType::CUBE);

    Material &m = model.add_component<Material>();
    m.uniforms_int["u_has_texture"] = 0;
    m.shader = shader_ptrs[0];

    if (args["texture"])
    {
        model.add_component<TextureComponent>(
            make_shared<ImgTexture>(args.get_value<string>("texture")));
        m.uniforms_int["u_has_texture"] = 1;
    }


    //     model.add_component<NativeScriptComponent>().bind<AsteroidController>();
    model.add_component<OrbitalComponent>();

    Transform &mt = model.get_component<Transform>();
    mt.position = init_model_pos;
    MeshComponent &mmc = model.get_component<MeshComponent>();

    mt.scale = vec3(1.0 / mmc.r_max);
    TRACE("r_max: {}", mmc.r_max);

	model.add_component<NativeScriptComponent>().bind<ModelController>();


    ObjHeader &header = model.get_component<MeshComponent>().header;
    if (header.loaded)
    {
		char overlay_text[5000];
        sprintf(overlay_text,
                "%s\nmethod: %s\n"
                "P   = %.6lf\n%s  = %.1lf\n%s  = %.1lf\ng   = %.1lf\n"
                "JD0 = %.3lf\n%s  = %d\n%s  = %d",
                header.get_item<string>("target").c_str(),
                header.get_item<string>("method").c_str(),
                header.get_item<double>("period[h]"), "\xef\xac\xa6",
                header.get_item<double>("lambda"), "\xef\x96\xa0",
                header.get_item<double>("beta"),
                header.get_item<double>("gamma"),
                header.get_item<double>("jd_gamma0"), "\xef\xa8\xb6",
                header.get_item<int>("number_of_triangles"), "\xef\xa5\x84",
                header.get_item<int>("number_of_vertices")
        );
		header_overlay.text = string(overlay_text);
    }
    else
    {
        header_overlay.text = "";
    }


    Entity runtime_observer = scene.create_entity("Observer");
    CameraComponent &rocp = runtime_observer.add_component<CameraComponent>(
        make_shared<OrthograficCamera>(cam_size_x, 1.0, 0.1, 10.));
    runtime_observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();

    rocp.camera->position = vec3(0., -5, 0.);
    rocp.camera->update_target(init_model_pos);

    Transform &rot = runtime_observer.get_component<Transform>();
    rot.position = rocp.camera->position;
    rot.speed = 8.;



    axes = scene.create_entity("xyz axes");
    axes.add_component<Material>("line_shader");
    Batch axes_batch;

    axes_batch.push_back(
        IndexedLine(vec3(0.), vec3(2. * mmc.r_max, 0., 0),
                    vec4(240 / 255., 84 / 255., 76 / 255., 1.)));

    axes_batch.push_back(
        IndexedLine(vec3(0.), vec3(0., 2. * mmc.r_max, 0),
                    vec4(128. / 255., 182. / 255., 23. / 255., 1.)));

    axes_batch.push_back(
        IndexedLine(vec3(0.), vec3(0., 0., 2. * mmc.r_max),
                    vec4(43. / 255., 143. / 255., 222. / 255., 1.)));

    axes.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(axes_batch.indexed_model));
    axes.get_component<SceneStatus>().casting_shadow = false;
    display_axes = false;



    scene.root_entity.add_child(model);

    scene.observer = runtime_observer;
    scene.target = model;


    if (args["projection"])
		make_projection_and_exit();
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

    if (info_overlay.timeout > 0.)
        info_overlay.timeout -= ts;

}


void MainLayer::on_imgui_render()
{
    // 	ImGui::GetCurrentContext()->IO.IniFilename = NULL;


    // 	ImGui::DockSpaceOverViewport();

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
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    if (display_header_info)
        header_overlay.display();
    if (display_help)
        help_overlay.display();

	info_overlay.display_timeout();
}


void MainLayer::on_event(Event &e)
{
	EventDispacher dispatcher(e);

	dispatcher.dispatch<KeyReleasedEvent>(
			bind(&MainLayer::on_key_released_event, this, placeholders::_1));

	dispatcher.dispatch<KeyPressedEvent>(
			bind(&MainLayer::on_key_pressed_event, this, placeholders::_1));

// 	dispatcher.dispatch<MouseMovedEvent>(
// 			bind(&MainLayer::on_mouse_moved_event, this, placeholders::_1));

	scene.controllers_events(e);
}


void MainLayer::on_key_pressed_event(KeyPressedEvent &event)
{
    const int key_code = event.get_key_code();

    if (key_code == GLFW_KEY_Z)
        if (Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
            info_overlay.reset_timeout("-Z axis view");
        else
            info_overlay.reset_timeout(" Z axis view");
    else if (key_code == GLFW_KEY_X)
        if (Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
            info_overlay.reset_timeout("-X axis view");
        else
            info_overlay.reset_timeout(" X axis view");
    else if (key_code == GLFW_KEY_Y)
        if (Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
            info_overlay.reset_timeout("-Y axis view");
        else
            info_overlay.reset_timeout(" Y axis view");

    else if (key_code == GLFW_KEY_I)
        display_header_info = !display_header_info;

    else if (key_code == GLFW_KEY_A)
    {
        if (display_axes)
        {
            display_axes = false;
            axes.detatch();
        }
        else
        {
            display_axes = true;
            scene.target.add_child(axes);
        }
    }
    else if (key_code == GLFW_KEY_H)
        display_help = true;
    else if (key_code == GLFW_KEY_P)
    {
        string filename = FileDialog::save_file("*png");
        if (!filename.empty())
            print_screen(filename);
    }
    else if (key_code == GLFW_KEY_B)
    {
        currnet_bg_color++;
        currnet_bg_color = currnet_bg_color % bg_colors.size();
        Application::set_bg_color(bg_colors[currnet_bg_color]);
    }
}

void MainLayer::on_key_released_event(KeyReleasedEvent &event)
{
	int key_code = event.get_key_code();
	if (key_code == GLFW_KEY_Q)
		Application::stop();
	else if (key_code == GLFW_KEY_H)
		display_help = false;
}


void MainLayer::on_mouse_moved_event(MouseMovedEvent &event)
{
}

void MainLayer::print_screen(string filename) {

  float *pixel_buffer_r =
      new float[(int)(viewport_panel_size.x * viewport_panel_size.y)];
  float *pixel_buffer_g =
      new float[(int)(viewport_panel_size.x * viewport_panel_size.y)];
  float *pixel_buffer_b =
      new float[(int)(viewport_panel_size.x * viewport_panel_size.y)];

  framebuffer->bind();
  glReadPixels(0, 0, viewport_panel_size.x, viewport_panel_size.y, GL_RED,
               GL_FLOAT, pixel_buffer_r);
  glReadPixels(0, 0, viewport_panel_size.x, viewport_panel_size.y, GL_GREEN,
               GL_FLOAT, pixel_buffer_g);
  glReadPixels(0, 0, viewport_panel_size.x, viewport_panel_size.y, GL_BLUE,
               GL_FLOAT, pixel_buffer_b);

  AoImage img(scene.target, scene.observer, viewport_panel_size.x,
              viewport_panel_size.y, false);
  img.update_data(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);
  img.save_png(filename);

  delete[] pixel_buffer_r;
  delete[] pixel_buffer_g;
  delete[] pixel_buffer_b;
}




void MainLayer::make_projection_and_exit()
{
    scene.update_scripts(0.0); // to initialze scripts
    ModelController *model_controller_script = dynamic_cast<ModelController *>(
        model.get_component<NativeScriptComponent>().instance);

    model.add_child(axes);


	// axes symbols

    string view = args.get_value<string>("projection");
    if (view == "x")
        model_controller_script->set_x_axis_view();
    else if (view == "y")
        model_controller_script->set_y_axis_view();
    else if (view == "z")
        model_controller_script->set_z_axis_view();
    else if (view == "nx")
        model_controller_script->set_negative_x_axis_view();
    else if (view == "ny")
        model_controller_script->set_negative_y_axis_view();
    else if (view == "nz")
        model_controller_script->set_negative_z_axis_view();
    else
	{
        cout << "wrong projection name" << endl;
		return;
	}

	Application::set_bg_color(vec4(1.0, 1.0, 1.0, 1.0));


	on_update(0.0);

	if (args["projection"])
	{
		string view = args.get_value<string>("projection");
		print_screen(args.get_value<string>("prefix") + "_" + view + ".png");
        Application::stop();
	}

}
