#include "svpch.h"
#include "Components.h"
#include "Framebuffer.h"
#include "MainLayer.h"
#include "CameraController.h"
#include "ImGuizmo.h"
#include "Math.h"
#include "ModelController.h"
#include "AsteroidController.h"
#include "Renderer.h"
#include "Utils/FileDialog.h"
#include "Utils/File.h"
#include "ObservatoryPanel.h"
#include "Utils/ObjHeader.h"
#include "Utils/Time.h"
#include "SceneSerializer.h"
#include <glm/gtc/type_ptr.hpp>

MainLayer::MainLayer(Args args)
{
    this->args = args;
    TRACE("MainLayer constructed");
}

MainLayer::~MainLayer()
{
    TRACE("MainLayer destructed");
}


void MainLayer::create_default_scene()
{
    vec3 init_model_pos(0., 1., 0.);
    Entity model = scene.create_entity("Model");
    if (args["model"])
        SceneHierarchyPanel::add_asteroid_from_file(
            model, args.get_value<string>("model"));
    else
	{
        model.add_component<MeshComponent>(ModelType::CUBE);
		model.add_component<Material>("tex_sha_ls").uniforms_int["u_has_texture"] = 0;
		model.add_component<NativeScriptComponent>().bind<AsteroidController>();
		model.add_component<OrbitalComponent>();
	}

    Transform &mt = model.get_component<Transform>();
    mt.scale = vec3(0.1);

    if (args["model-pos"])
    {
        vector<float> pos = args.get_vec_values<float>("model-pos");
        mt.position = vec3(pos[0], pos[1], pos[2]);
    }
    else
        mt.position = init_model_pos;


    Entity runtime_observer = scene.create_entity("Observer");
    CameraComponent &rocp = runtime_observer.add_component<CameraComponent>(
        //         make_shared<PerspectiveCamera>(
        //             radians(45.0), window->width / (float)window->height,
        //             0.01, 500.0));
        make_shared<OrthograficCamera>(0.2, 1.0, 0.1, 10.));
    runtime_observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();

    runtime_observer.add_component<Material>("color_shader")
        .uniforms_vec4["u_color"] =
        vec4(40 / 256., 185 / 256., 240 / 256., 1.0);
    MeshComponent &romc =
        runtime_observer.add_component<MeshComponent>(ModelType::ICO_SPHERE);
    rocp.camera->position = vec3(sqrt(2.) / 2., sqrt(2.) / 2., 0.);
    if (args["observer-pos"])
    {
        vector<float> pos = args.get_vec_values<float>("observer-pos");
        rocp.camera->position = vec3(pos[0], pos[1], pos[2]);
    }

    rocp.camera->update_target(init_model_pos);
    // 	romc.vao->draw_type = GL_LINES;
    Transform &rot = runtime_observer.get_component<Transform>();
    rot.position = rocp.camera->position;
    rot.speed = 8.;
    rot.scale = vec3(0.05);

    Entity light = scene.create_entity("Light");
    light.add_component<MeshComponent>(ModelType::ICO_SPHERE);
    light.add_component<Material>("color_shader").uniforms_vec4["u_color"] =
        vec4(245. / 256, 144. / 256, 17. / 256, 1.0);
    light.add_component<LightComponent>();

    light.add_component<FramebufferComponent>(
        make_shared<Framebuffer>(2048, 2048, DEPTH_ATTACHMENT));

    light.add_component<CameraComponent>(
        //         make_shared<PerspectiveCamera>(
        //             radians(25.0), window->width / (float)window->height,
        //             0.01, 50.0));
        make_shared<OrthograficCamera>(1., 1., 0.01, 10.));
    light.get_component<Transform>().scale = vec3(0.1);

    scene.light = light;

    scene.root_entity.add_child(model);
    scene.root_entity.add_child(light);
    scene.root_entity.add_child(runtime_observer);

    scene.observer = runtime_observer;
    scene.target = model;
}


void MainLayer::on_attach()
{
    auto window = Application::get_window();
    Renderer::set_line_width(2.);
    Renderer::enable_blend();

	if (args["scene"])
	{
		SceneSerializer scene_serializer(&scene);
		string filename = args.get_value<string>("scene");

		if (File::is_extension(filename, "scene"))
			scene_serializer.deserialize(filename);
		else
		{
			cout << "Wrong Scene file extension" << endl;
			create_default_scene();
		}
	}
	else if (!args["empty"])
		create_default_scene();



    grid = ui_scene.create_entity("grid");
    grid.add_component<Material>("line_shader");
    grid.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(create_grid(10., 1., 0.2), true));
    ui_scene.root_entity.add_child(grid);

    editor_camera =
        EditorCamera(radians(45.), window->width / (float)window->height, 0.01,
                     500.0, vec3(6.3, -3., 5.12));
    ms_framebuffer =
        new Framebuffer(window->width, window->height,
                        COLOR_ATTACHMENT | DEPTH_ATTACHMENT | MULTISAMPLING);

    framebuffer = new Framebuffer(window->width, window->height,
                                  COLOR_ATTACHMENT | DEPTH_ATTACHMENT);

    time_panel = TimePanel(&scene);
    scene_hierarchy_panel = SceneHierarchyPanel(&scene, &time_panel.julian_day);
    observatory_panel =
        ObservatoryPanel(this, &obs_pack);
    observe_panel = ObservePanel(this);
}

void MainLayer::on_event(Event &e)
{
    EventDispacher dispatcher(e);

    dispatcher.dispatch<WindowResizeEvent>(
        bind(&MainLayer::on_window_resize_event, this, placeholders::_1));

    dispatcher.dispatch<KeyReleasedEvent>(
        bind(&MainLayer::on_key_released_event, this, placeholders::_1));

    scene.controllers_events(e);

    if (mode == Mode::EDITOR)
        editor_camera.on_event(e);
}

void MainLayer::on_key_released_event(KeyReleasedEvent &event)
{
    int key_code = event.get_key_code();

    if (mode == Mode::EDITOR)
    {
        if (key_code == GLFW_KEY_Q)
            Application::stop();

        if (key_code == GLFW_KEY_G)
            guizmo_type = ImGuizmo::OPERATION::TRANSLATE;
        else if (key_code == GLFW_KEY_R)
            guizmo_type = ImGuizmo::OPERATION::ROTATE;
        else if (key_code == GLFW_KEY_S)
            guizmo_type = ImGuizmo::OPERATION::SCALE;
        else if (key_code == GLFW_KEY_ESCAPE)
            guizmo_type = -1;
    }

    if (mode == Mode::RUNTIME)
        if (key_code == GLFW_KEY_Q)
            toggle_mode();
}

void MainLayer::on_window_resize_event(WindowResizeEvent &event)
{
    //     ivec2 size = event.get_size();
}

void MainLayer::on_update(double time_delta)
{
    // 	int history_len = 100;
    // 	for (int i = history_len-1; i > 0; i--)
    // 	{
    // 		fps_history[i] = fps_history[i - 1];
    // 	}
    // 	fps_history[0] = 1./time_delta;
    //
    // 	fps = 0.;
    // 	for (int i = 0; i < history_len; i++)
    // 	{
    // 		fps += fps_history[i];
    // 	}
    // 	fps /= history_len;

    ASSERT(mode < Mode::NUM_MODES, "Wrong Mode");

    if (shadow_map && scene.light) // render to shadowmap
    {
        ASSERT(scene.light.has_component<FramebufferComponent>(),
               "Scene light doesn't have Framebuffer Component");
        ASSERT(scene.light.has_component<CameraComponent>(),
               "Scene light doesn't have Camera Component");

        SceneStatus &lss = scene.light.get_component<SceneStatus>();
        bool tmp_render = lss.render;
        lss.render = false;

        Transform &lt = scene.light.get_component<Transform>();
        auto lc = scene.light.get_component<CameraComponent>().camera;

        lc->position = lt.position;
        if (scene.target)
            lc->update_target(scene.target.get_component<Transform>().position);
        lc->aspect = 1;

        auto fb = scene.light.get_component<FramebufferComponent>().framebuffer;
        fb->bind();
        fb->clear();

        Entity tmp_scene_observer = scene.observer;
        scene.observer = scene.light;

        if (mode == Mode::EDITOR)
            // musi być zeby textura cieni byla aktualna,
            // inaczej jest tam info o nieniu z ostatniego runtime pass,
            // a shader jest taki sam w obu modach bierze nieaktualne info o
            // cieniach
            scene.on_update_editor(time_delta, editor_camera);
        else if (mode == Mode::RUNTIME)
            scene.on_update_shadow();

        fb->bind_depth_texture(1);

        lss.render = tmp_render;
        scene.observer = tmp_scene_observer;
    }


    if (multisampling)
    {
        ms_framebuffer->bind();
        ms_framebuffer->clear();
    }
    else
    {
        framebuffer->bind();
        framebuffer->clear();
    }

    if (mode == Mode::EDITOR)
    {
        editor_camera.on_update(time_delta);
        scene.on_update_editor(time_delta, editor_camera);
        ui_scene.on_update_editor(time_delta, editor_camera);
    }
    else if (mode == Mode::RUNTIME)
    {
        scene.on_update_runtime(time_delta);
    }

    if (multisampling)
        Framebuffer::blit(ms_framebuffer, framebuffer);
    Renderer::bind_default_framebuffer();
}

void MainLayer::on_imgui_render()
{
    ImGui::DockSpaceOverViewport();
    menu_bar();
    scene_window();
    //     orbital_parameters_panel();

    if (show_scene_options)
        scene_options_panel();

    if (show_imgui_demo)
        ImGui::ShowDemoWindow();

    time_panel.on_imgui_render();
    scene_hierarchy_panel.on_imgui_render();
    observe_panel.on_imgui_render();
    observatory_panel.on_imgui_render();
}

void MainLayer::on_detach()
{
    delete ms_framebuffer;
    delete framebuffer;
}

void MainLayer::toggle_mode()
{
    if (mode == Mode::EDITOR)
        set_runtime_mode();
    else if (mode == Mode::RUNTIME)
        set_editor_mode();
}

void MainLayer::set_editor_mode()
{
    mode = Mode::EDITOR;
    Application::get_window()->set_cursor_normal();
    // TODO: make all objects visible in Editor mode
    scene.observer.get_component<SceneStatus>().render = true;
    scene.observer.get_component<Transform>().position =
        scene.observer.get_component<CameraComponent>().camera->position;
}

void MainLayer::set_runtime_mode()
{
    if (!scene.observer || !scene.target)
        return;

    if (!scene.observer.has_component<CameraComponent>())
        return;

    mode = Mode::RUNTIME;
    // 		scene.observer.get_component<CameraComponent>().camera->update_target(
    // 			model.get_component<Transform>().position);
    guizmo_type = -1;
    //     Application::get_window()->set_cursor_disabled();

    scene.observer.get_component<SceneStatus>().render = false;

    shared_ptr<Camera> camera =
        scene.observer.get_component<CameraComponent>().camera;
    camera->position = scene.observer.get_component<Transform>().position;
    camera->update_target(scene.target.get_component<Transform>().position);

    scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
}

void MainLayer::menu_bar()
{
    ImGuiIO &io = ImGui::GetIO();
    auto bold_font = io.Fonts->Fonts[0];

    ImGui::PushFont(bold_font);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save scene"))
            {
                SceneSerializer scene_serializer(&scene);
                string filename = FileDialog::save_file("*.scene");
                scene_serializer.serialize(filename);
            }

            if (ImGui::MenuItem("Load scene"))
            {
                SceneSerializer scene_serializer(&scene);
                string filename = FileDialog::open_file("*.scene");
                scene_serializer.deserialize(filename);
                cout << "loading scene" << endl;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Quit", "Q"))
                Application::stop();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("ImGui Demo", NULL, &show_imgui_demo);
            ImGui::MenuItem("Scene Options", NULL, &show_scene_options);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::BeginMenu("BG Color"))
            {
                float color[3];
                vec4 app_color = Application::get_bg_color();
                color[0] = app_color.r;
                color[1] = app_color.g;
                color[2] = app_color.b;

                ImGui::ColorPicker3("BG Color", color);

                app_color.r = color[0];
                app_color.g = color[1];
                app_color.b = color[2];

                Application::set_bg_color(app_color);

                ImVec4 default_bg(41 / 256., 46 / 256., 48 / 256., 1.0);
                ImGui::Text("Set default color");
                if (ImGui::ColorButton("Default color", default_bg,
                                       ImGuiColorEditFlags_NoPicker |
                                           ImGuiColorEditFlags_AlphaPreviewHalf,
                                       ImVec2(60, 40)))
                {
                    app_color.r = default_bg.x;
                    app_color.g = default_bg.y;
                    app_color.b = default_bg.z;
                    Application::set_bg_color(app_color);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Observations"))
        {
            if (ImGui::MenuItem("New Storage"))
            {
                obs_pack.add_new("untitled");
            }

            if (ImGui::MenuItem("Import Storage"))
            {
                load_obs_storage(
                    FileDialog::open_file("*.storage"));
            }

            if (ImGui::MenuItem("Import LC file"))
            {
                load_lc_file(FileDialog::open_file("*.txt"));
            }


            if (ImGui::MenuItem("Save Storage"))
            {
                string filename = FileDialog::save_file("*.storage");
                if (filename != "")
                {
                    obs_pack.save_current(filename);
                }
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Delete Observations", NULL, false, true))
            {
                obs_pack.delete_current_observations();
            }

            if (ImGui::MenuItem("Delete Storage", NULL, false, true))
            {
                obs_pack.delete_current();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    ImGui::PopFont();
}

void MainLayer::scene_window()
{
    //     ImGui::Begin("depth map");
    //     long int shadow_tex_id = light.get_component<FramebufferComponent>()
    //                                  .framebuffer->get_depth_attachment_id();
    //     ImGui::Image((void *)shadow_tex_id, ImVec2(300, 300), ImVec2(0, 1),
    //                  ImVec2(1, 0));
    //     ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
    ImGui::Begin("Scene");

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
    {
        viewport_panel_size.x = vps.x;
        viewport_panel_size.y = vps.y;

        scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
        editor_camera.on_resize(viewport_panel_size.x, viewport_panel_size.y);
        ms_framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        Renderer::bind_default_framebuffer();
    }

    long int tex_id = framebuffer->get_color_attachment_id();
    ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.y), ImVec2(0, 1),
                 ImVec2(1, 0));


    // Gizmos
    Entity selected_entity = scene.selected_entity; // TODO: created selection
    if (selected_entity && selected_entity.get_parent() && guizmo_type != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                          windowWidth, windowHeight);

        // Editor camera
        const glm::mat4 &camera_projection = editor_camera.get_perspective();
        glm::mat4 camera_view = editor_camera.get_view();

        // Entity transform
        Transform &tc = selected_entity.get_component<Transform>();
        glm::mat4 transform = tc.get_world_tansform();

        // Snapping
        bool snap = Input::is_key_pressed(GLFW_KEY_LEFT_CONTROL);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // Snap to 45 degrees for rotation
        if (guizmo_type == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = {snapValue, snapValue, snapValue};

        int manipulation_ref_frame = ImGuizmo::WORLD;
        if (guizmo_type == ImGuizmo::OPERATION::SCALE)
            manipulation_ref_frame = ImGuizmo::LOCAL;

        mat4 delta_matrix;
        ImGuizmo::Manipulate(
            glm::value_ptr(camera_view), glm::value_ptr(camera_projection),
            (ImGuizmo::OPERATION)guizmo_type,
            (ImGuizmo::MODE)manipulation_ref_frame, glm::value_ptr(transform),
            glm::value_ptr(delta_matrix), snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            Entity &parent = selected_entity.get_parent();
            // 			if (!parent)  // no parent, root
            Transform &ptc = parent.get_component<Transform>();

            glm::vec3 translation, rotation, scale;
            glm::vec3 delta_translation, delta_rotation, delta_scale;

            Math::decompose_transform(delta_matrix, delta_translation,
                                      delta_rotation, delta_scale);

            Math::decompose_transform(transform, translation, rotation, scale);

            if (guizmo_type == ImGuizmo::OPERATION::TRANSLATE)
            {
                vec4 pos = vec4(tc.position, 1.0);
                pos += glm::inverse(ptc.get_world_tansform()) *
                       vec4(delta_translation, 0.0);
                tc.position = vec3(pos);
            }
            else if (guizmo_type == ImGuizmo::OPERATION::ROTATE)
            {
                vec4 rot = vec4(tc.rotation, 0.0);
                rot += glm::inverse(ptc.get_world_tansform()) *
                       vec4(delta_rotation, 0.0);
                tc.rotation = vec3(rot);
            }
            else if (guizmo_type == ImGuizmo::OPERATION::SCALE)
                tc.scale = scale;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void MainLayer::scene_options_panel()
{

    ImGui::Begin("Scene Options");

    if (ImGui::RadioButton("Translate", guizmo_type == ImGuizmo::TRANSLATE))
        guizmo_type = ImGuizmo::TRANSLATE;

    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", guizmo_type == ImGuizmo::ROTATE))
        guizmo_type = ImGuizmo::ROTATE;

    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", guizmo_type == ImGuizmo::SCALE))
        guizmo_type = ImGuizmo::SCALE;

    ImGui::SameLine();
    if (ImGui::RadioButton("None", guizmo_type == -1))
        guizmo_type = -1;

    if (ImGui::Checkbox("show grid", &show_grid))
    {
        grid.detatch();
        if (show_grid)
            ui_scene.root_entity.add_child(grid);
    }


    ImGui::End();
}

IndexedModel MainLayer::create_grid(float size, float sep, float alpha)
{
    Batch grid_batch;

    for (float i = -size; i <= size; i += sep)
    {
        if (i == 0.)
            continue;

        grid_batch.push_back(IndexedLine(vec3(-size, i, 0.), vec3(size, i, 0.),
                                         vec4(0.3, 0.3, 0.3, alpha)));

        grid_batch.push_back(IndexedLine(vec3(i, -size, 0.), vec3(i, size, 0.),
                                         vec4(0.3, 0.3, 0.3, alpha)));
    }

    grid_batch.push_back(
        IndexedLine(vec3(-size, 0., 0.), vec3(size, 0., 0.),
                    vec4(245. / 256, 74. / 256, 29. / 256, alpha)));
    grid_batch.push_back(
        IndexedLine(vec3(0., -size, 0.), vec3(0., size, 0.),
                    vec4(28. / 256, 157. / 256, 51. / 256, alpha)));

    return grid_batch.indexed_model;
}


void MainLayer::make_lightcurve(LightcurveSeries *lightcurves, int num_points)
{
	Entity &target = scene.target;
	Entity &observer = scene.observer;
    Transform &tt = target.get_component<Transform>();
	if (!target.has_component<OrbitalComponent>())
	{
		cout << "target does not have orbital component" << endl;
		return;
	}

    OrbitalComponent &oc = target.get_component<OrbitalComponent>();

    int width = 256;
    int height = 256;
    float *pixel_buffer = new float[width * height];
    Lightcurve *lc = new Lightcurve(target, observer, num_points);
    vec4 bg_color = Application::get_bg_color();

    observer.get_component<CameraComponent>().camera->update_target(
        tt.position);

    Mode prev_mode = mode;
    set_runtime_mode();

    Application::set_bg_color(vec4(0., 0., 0., 1.));
    scene.on_resize(width, height);
    ms_framebuffer->resize(width, height);
    framebuffer->resize(width, height);

    for (int i = 0; i < num_points; i++)
    {
        if (oc.rotation_speed != 0)
            on_update(2 * M_PI / num_points / oc.rotation_speed);
        else
            on_update(2 * M_PI / num_points);

        framebuffer->bind();
        glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, pixel_buffer);

        double flux = 0.0;
        for (int j = 0; j < width * height; j++)
            flux += pixel_buffer[j];
// 		printf("%.16lf\n", flux);
        lc->push_flux((double)i/num_points * 2*M_PI, flux);
    }

    Entity ghost_observer = ui_scene.create_entity("ghost observer");
    Entity ghost_target = ui_scene.create_entity("ghost target");
    lc->add_ghosts(ghost_observer, ghost_target, "flat_shader");

    lc->julian_day = time_panel.julian_day;


    lightcurves->push(lc);
    lightcurves->detach_all_ghosts();

    ui_scene.root_entity.add_child(lc->ghost_target);
    ui_scene.root_entity.add_child(lc->ghost_observer);


    scene.on_resize(viewport_panel_size.x,
                           viewport_panel_size.y);
    ms_framebuffer->resize(viewport_panel_size.x,
                                  viewport_panel_size.y);
    framebuffer->resize(viewport_panel_size.x,
                               viewport_panel_size.y);

    Application::set_bg_color(bg_color);
    if (prev_mode == Mode::EDITOR)
        set_editor_mode();
    delete[] pixel_buffer;
}


void MainLayer::make_ao_image(ImageSeries *ao_images, int ao_size, bool earth_tilt,
		vec4 bg_color)
{
	Entity &target = scene.target;
	Entity &observer = scene.observer;
    int ao_width = ao_size;
    int ao_height = ao_size;

    float *pixel_buffer_r = new float[ao_width * ao_height];
    float *pixel_buffer_g = new float[ao_width * ao_height];
    float *pixel_buffer_b = new float[ao_width * ao_height];

    // 	AoImage ao_image(width, height);
    vec4 tmp_bg_color = Application::get_bg_color();

    observer.get_component<CameraComponent>().camera->update_target(
        target.get_component<Transform>().position);

    Mode prev_mode = mode;
    set_runtime_mode();

    Application::set_bg_color(bg_color);
    scene.on_resize(ao_width, ao_height);
    ms_framebuffer->resize(ao_width, ao_height);
    framebuffer->resize(ao_width, ao_height);

    if (earth_tilt)
        observer.get_component<CameraComponent>().camera->up =
            vec3(0., 0.39774, 0.917498);

    // make image
    on_update(0.);
    framebuffer->bind();
    glReadPixels(0, 0, ao_width, ao_height, GL_RED, GL_FLOAT, pixel_buffer_r);
    glReadPixels(0, 0, ao_width, ao_height, GL_GREEN, GL_FLOAT, pixel_buffer_g);
    glReadPixels(0, 0, ao_width, ao_height, GL_BLUE, GL_FLOAT, pixel_buffer_b);

    AoImage *ao = new AoImage(target, observer, ao_width, ao_height);
//     ao->texture->update(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);
	ao->update_data(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);
	ao->update_texture();

    Entity ghost_observer = ui_scene.create_entity("ghost observer");
    Entity ghost_target = ui_scene.create_entity("ghost target");
    ao->add_ghosts(ghost_observer, ghost_target, "flat_shader");
    ao->julian_day = time_panel.julian_day;

    ao_images->push(ao);
    ao_images->detach_all_ghosts();

    ui_scene.root_entity.add_child(ao->ghost_target);
    ui_scene.root_entity.add_child(ao->ghost_observer);


    scene.on_resize(viewport_panel_size.x,
                           viewport_panel_size.y);
    ms_framebuffer->resize(viewport_panel_size.x,
                                  viewport_panel_size.y);
    framebuffer->resize(viewport_panel_size.x,
                               viewport_panel_size.y);

    Application::set_bg_color(tmp_bg_color);

    if (prev_mode == Mode::EDITOR)
        set_editor_mode();

    delete[] pixel_buffer_r;
    delete[] pixel_buffer_g;
    delete[] pixel_buffer_b;
}


void MainLayer::make_radar_image(ImageSeries *radar_images, int dd_size, float angular_speed)
{
    Entity &target = scene.target;
    Entity &observer = scene.observer;

    int frame_width = dd_size * 5;
    int frame_height = dd_size * 5;

    float *pixel_buffer_r = new float[frame_width * frame_height];
    float *pixel_buffer_normal = new float[frame_width * frame_height];
    float *pixel_buffer_depth = new float[frame_width * frame_height];

    vec4 tmp_bg_color = Application::get_bg_color();

    Material &mat = target.get_component<Material>();
    shared_ptr<Shader> tmp_shader = mat.shader;
    mat.shader = Application::shaders["radar"];
    mat.uniforms_float["u_factor"] = angular_speed;

    CameraComponent &camera_comp = observer.get_component<CameraComponent>();
    shared_ptr<Camera> tmp_cam = camera_comp.camera;


    // create view box that fits the target perfectly
    Transform &tt = target.get_component<Transform>();
    Transform &ot = observer.get_component<Transform>();
    MeshComponent &mc = target.get_component<MeshComponent>();

    float max_scale = glm::max(tt.scale.x, tt.scale.y);
    max_scale = glm::max(max_scale, tt.scale.z);

    float near =
        glm::length(tt.position - ot.position) - (max_scale * mc.r_max);
    float far = glm::length(tt.position - ot.position) + (max_scale * mc.r_max);

    //    	TRACE("near: {}, far: {}, rmax: {}", near, far, mc.r_max);
    camera_comp.camera =
        make_shared<OrthograficCamera>(max_scale * mc.r_max, 1., near, far);


    Mode prev_mode = mode;
    set_runtime_mode();

    scene.on_resize(frame_width, frame_height);

    multisampling = false;
    framebuffer->resize(frame_width, frame_height);
    Application::set_bg_color(vec4(0, 0, 0, 0));

    framebuffer->clear();

    on_update(0.);
    framebuffer->bind();
    glReadPixels(0, 0, frame_width, frame_height, GL_RED, GL_FLOAT,
                 pixel_buffer_r);
    glReadPixels(0, 0, frame_width, frame_height, GL_GREEN, GL_FLOAT,
                 pixel_buffer_normal);
    glReadPixels(0, 0, frame_width, frame_height, GL_BLUE, GL_FLOAT,
                 pixel_buffer_depth);

    RadarImage *rimg =
        new RadarImage(target, observer, dd_size, dd_size, near, far, -1, 1);
    rimg->construct_delay_doppler(pixel_buffer_r, pixel_buffer_depth,
                                  pixel_buffer_normal, frame_width,
                                  frame_height);
    rimg->update_texture();

    Entity ghost_observer = ui_scene.create_entity("ghost observer");
    Entity ghost_target = ui_scene.create_entity("ghost target");
    rimg->add_ghosts(ghost_observer, ghost_target, "flat_shader");
    rimg->julian_day = time_panel.julian_day;

    radar_images->push(rimg);
    radar_images->detach_all_ghosts();

    ui_scene.root_entity.add_child(rimg->ghost_target);
    ui_scene.root_entity.add_child(rimg->ghost_observer);

    target.get_component<Material>().shader = tmp_shader;
    Application::set_bg_color(tmp_bg_color);
    scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
    ms_framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
    framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);

    if (prev_mode == Mode::EDITOR)
        set_editor_mode();

    multisampling = true;

    camera_comp.camera = tmp_cam;

    delete[] pixel_buffer_r;
    delete[] pixel_buffer_depth;
    delete[] pixel_buffer_normal;
}



void MainLayer::load_obs_storage(string filepath)
{
    if (filepath == "")
        return;

	vector<ObsPoint> obs_points = ObsStoragePack::import_obs_points(filepath);
	if (obs_points.size() > 0)
	{
		obs_pack.add_new(File::remove_extension(File::file_base(filepath)));
		observe_obs_points(obs_points);
	}
	else
        cout << "Adding Storage Failed" << endl;
}


void MainLayer::load_lc_file(string filepath)
{
	/// The lightcurves are in phase space. The 0 phase in the plot is the phase
	/// of the model as it would be observed at this time. So, the observed points
	/// always start at 0

	if (filepath == "")
		return;

    char line[1000];
    int num_lcs = 0;
    vector<ObsPoint> obs_points;

    FILE *in = fopen(filepath.c_str(), "r");

    fgets(line, 999, in);
    sscanf(line, "%d", &num_lcs);

    if (num_lcs > 0)
    {
        obs_pack.add_new(File::remove_extension(File::file_base(filepath)));
        obs_pack.add_series<LightcurveSeries>("obs_lightcurves");
    }
    else
        return;

    OrbitalComponent &oc =
        scene.target.get_component<OrbitalComponent>();

    for (int i = 0; i < num_lcs; i++)
    {
        fgets(line, 999, in);
        int type = 0;
        int num_pts = 0;
        sscanf(line, "%d %d", &num_pts, &type);

        Lightcurve *lc =
            new Lightcurve(scene.target, scene.observer, num_pts);

		double phase_offset = 0;
		double jd_start = 0;
        for (int j = 0; j < num_pts; j++)
        {
            double jd, flux;
            dvec3 sun, earth;
            fgets(line, 999, in);
            sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &jd, &flux, &sun.x,
                   &sun.y, &sun.z, &earth.x, &earth.y, &earth.z);

            if (j == 0)
            {
				jd_start = jd;
                lc->julian_day = jd;

                ObsPoint p;
                p.jd = jd;
                p.observer_pos = earth - sun;
                p.target_pos = -sun;
                p.obs_type = ObsType::LC;

                obs_points.push_back(p);
            }

			double phase = (jd - jd_start) / (oc.rot_period / 24.) *2 * M_PI;
			while (phase >= 2*M_PI)
				phase -= 2*M_PI;
			while (phase < 0)
				phase += 2* M_PI;

            lc->push_flux(phase, flux);

        }

        lc->sort();
        obs_pack.get_series<LightcurveSeries>("obs_lightcurves")->push(lc);
    }
    fclose(in);

    if (obs_points.size() > 0)
    {
        observe_obs_points(obs_points);
    }
    else
        cout << "Adding Storage Failed" << endl;
}


void MainLayer::observe_obs_points(const vector<ObsPoint> obs_points)
{
    OrbitalComponent &oc =
        scene.target.get_component<OrbitalComponent>();
    double tmp_rotation_phase = oc.rotation_phase;

    vec3 &target_rotation =
        scene.target.get_component<Transform>().rotation;
    vec3 tmp_rotation = target_rotation;
    vec3 &target_pos = scene.target.get_component<Transform>().position;
    vec3 tmp_target_pos = target_pos;
    vec3 &observer_pos =
        scene.observer.get_component<Transform>().position;
    vec3 tmp_observer_pos = observer_pos;

    double tmp_julian_day = time_panel.julian_day;

	vec4 ao_bg_color = vec4(0., 0., 0., 1.);
	bool earth_tilt = true;
	float angular_speed = 10.0;


    for (ObsPoint p : obs_points)
    {
        time_panel.julian_day = p.jd;

        oc.set_rot_phase_at_jd(p.jd);
        target_rotation = oc.xyz_from_lbg();

        target_pos = p.target_pos;
        observer_pos = p.observer_pos;

        if (p.obs_type & ObsType::LC)
        {
            if (!obs_pack.get_series<LightcurveSeries>("lightcurves"))
                obs_pack.add_series<LightcurveSeries>("lightcurves");

            make_lightcurve(
                obs_pack.get_series<LightcurveSeries>("lightcurves"),
                p.lc_num_points);
        }

        if (p.obs_type & ObsType::AO)
        {
            if (!obs_pack.get_series<ImageSeries>("ao_images"))
                obs_pack.add_series<ImageSeries>("ao_images");

            make_ao_image(obs_pack.get_series<ImageSeries>("ao_images"),
                          p.ao_size, earth_tilt, ao_bg_color);
        }

        if (p.obs_type & ObsType::RADAR)
        {
            if (!obs_pack.get_series<ImageSeries>("radar_images"))
                obs_pack.add_series<ImageSeries>("radar_images");

            make_radar_image(
                obs_pack.get_series<ImageSeries>("radar_images"),
                p.radar_size, angular_speed);
        }
    }
    target_pos = tmp_target_pos;
    observer_pos = tmp_observer_pos;
    target_rotation = tmp_rotation;

    time_panel.julian_day = tmp_julian_day;
}


void MainLayer::set_target_and_observer(Observation *obs)
{
    vec3 gtp = obs->target_transform.position;
    vec3 gop = obs->observer_transform.position;

    scene.target = obs->target;
    scene.observer = obs->observer;

    scene.target.get_component<Transform>() = obs->target_transform;
    scene.observer.get_component<Transform>() = obs->observer_transform;

    if (scene.observer.has_component<OrbitalComponent>())
        scene.observer.get_component<OrbitalComponent>() =
            obs->target_orbital_component;

    shared_ptr<Camera> camera =
        scene.observer.get_component<CameraComponent>().camera;
    camera->position = gop;
    camera->update_target(gtp);
}


void MainLayer::set_current_ghosts()
{
    obs_pack.current_obs_lambda([this](Observation *current_obs) {
        if (current_obs->ghost_target && current_obs->ghost_observer)
        {
            ui_scene.root_entity.add_child(current_obs->ghost_target);
            ui_scene.root_entity.add_child(current_obs->ghost_observer);
        }
    });
}


void MainLayer::set_ghosts(Observation* obs)
{
    ui_scene.root_entity.add_child(obs->ghost_target);
    ui_scene.root_entity.add_child(obs->ghost_observer);
}
