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
		model.add_component<Material>("tex_sha").uniforms_int["u_has_texture"] = 0;
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
        ObservatoryPanel(this, &time_panel.julian_day, &obs_storage);
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
                obs_storage.add_new("untitled");
            }

            if (ImGui::MenuItem("Import Storage"))
            {
                observatory_panel.load_obs_storage(
                    FileDialog::open_file("*.yaml"));
            }


            if (ImGui::MenuItem("Save Storage"))
            {
                string filename = FileDialog::save_file("*.yaml");
                if (filename != "")
                {
                    obs_storage.save(filename);
                }
            }
            ImGui::Separator();

            bool enabled = (obs_storage.get_current_points_size() > 0);
            if (ImGui::MenuItem("Observe All Points", NULL, false, enabled))
            {
                if (!scene.target.has_component<OrbitalComponent>())
                {
                    cout << "Target does not have an Orbital Component" << endl;
                }
                else
                {
                    observatory_panel.observe_points();
                }
            }

            if (ImGui::MenuItem("Delete Observations", NULL, false, true))
            {
                obs_storage.delete_current_observations();
            }

            if (ImGui::MenuItem("Delete Storage", NULL, false, true))
            {
                obs_storage.delete_current();
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
