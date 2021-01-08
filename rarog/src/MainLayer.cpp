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
#include "ObservatoryPanel.h"
#include <glm/gtc/type_ptr.hpp>

MainLayer::MainLayer(int argc, char *argv[])
{
    arg_handler.addArgument("m", "model", "", "path to OBJ model");

    arg_handler.parseArguments(argc, argv);

    if (!arg_handler.isSpecified("model"))
    {
        cout << "Model file not specified." << endl;
        // 		cout << "type rarog --help to se help" << endl;
        // 		exit(1);
    }

    TRACE("MainLayer constructed");
}

MainLayer::~MainLayer()
{
}

void MainLayer::on_attach()
{
	observatory_panel = new ObservatoryPanel(this);

    auto window = Application::get_window();
    Renderer::set_line_width(2.);
    vec3 init_model_pos(0., 5., 0.);


    shared_ptr<VertexArrayObject> model_vao;
    if (arg_handler.isSpecified("model"))
    {
        string filename = arg_handler.args["model"].to_str();
        if (filename.compare(filename.rfind("."), 4, ".obj") == 0)
            model_vao = make_shared<VertexArrayObject>(
                IndexedModelObj(filename, NormalIndexing::PER_VERTEX));
        else if (filename.compare(filename.rfind("."), 4, ".shp") == 0)
            model_vao = make_shared<VertexArrayObject>(
                IndexedModelShp(filename, NormalIndexing::PER_FACE));
    }
    else
        model_vao =
            make_shared<VertexArrayObject>(IndexedCube(vec3(-0.5), vec3(1.)));

    grid = scene.create_entity("grid");
    grid.add_component<Material>(Application::shaders["line_shader"]);
    grid.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(create_grid(100., 5., 0.2), true));

    runtime_observer = scene.create_entity("Observer");
    CameraComponent &rocp = runtime_observer.add_component<CameraComponent>(
//         make_shared<PerspectiveCamera>(
//             radians(45.0), window->width / (float)window->height, 0.01, 500.0));
		   make_shared<OrthograficCamera>(
				   6., 1.0, 0.1, 20.));
    runtime_observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();

	runtime_observer.add_component<Material>(Application::shaders["color_shader"])
				.uniforms_vec4[ "u_color"] = vec4(40/256., 185/256., 240/256., 1.0);
	runtime_observer.add_component<MeshComponent>(make_shared<VertexArrayObject>(
			IndexedIcoSphere(vec3(0.), vec3(0.3))));
    rocp.camera->position = vec3(3., 3., 0.);
    rocp.camera->update_target(init_model_pos);

    Transform &rot = runtime_observer.get_component<Transform>();
    rot.position = rocp.camera->position;
    rot.speed = 8.;

    light = scene.create_entity("Light");
    light.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(0.5))));
    light.add_component<Material>(Application::shaders["color_shader"])
        .uniforms_vec4["u_color"] =
        vec4(245. / 256, 144. / 256, 17. / 256, 1.0);
	light.add_component<LightComponent>();

    light.add_component<FramebufferComponent>(
			make_shared<Framebuffer>(1024, 1024, DEPTH_ATTACHMENT));

	light.add_component<CameraComponent>(
//         make_shared<PerspectiveCamera>(
//             radians(25.0), window->width / (float)window->height, 0.01, 50.0));
			make_shared<OrthograficCamera>(10., 1., 0.01, 20.));

    model = scene.create_entity("Model");
    model.add_component<Material>(Application::shaders["tex_sha"])
        .uniforms_int["u_has_texture"] = 0;
    model.add_component<MeshComponent>(model_vao);
    model.add_component<NativeScriptComponent>().bind<AsteroidController>();
//     model.add_component<NativeScriptComponent>().bind<ModelController>();
	Transform &mt = model.get_component<Transform>();
	mt.rotation_speed = 2 * M_PI / 10.;
    mt.position = init_model_pos;



// 	Entity box = scene.create_entity("box");
// 	box.add_component<Material>(Application::shaders["basic_shader"]);
// 	box.add_component<MeshComponent>(make_shared<VertexArrayObject>(IndexedCube()));

    scene.root_entity.add_child(model);
    scene.root_entity.add_child(light);
    scene.root_entity.add_child(runtime_observer);
//     scene.root_entity.add_child(box);

    ui_scene.root_entity.add_child(grid);

	scene.observer = runtime_observer;


    editor_camera =
        EditorCamera(radians(45.), window->width / (float)window->height, 0.01,
                     500.0, vec3(6.3, -3., 5.12));

    scene_hierarchy_panel = SceneHierarchyPanel(&scene);

    ms_framebuffer =
        new Framebuffer(window->width, window->height,
                        COLOR_ATTACHMENT | DEPTH_ATTACHMENT | MULTISAMPLING);

    framebuffer = new Framebuffer(window->width, window->height,
                                  COLOR_ATTACHMENT | DEPTH_ATTACHMENT);
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

	if (shadow_map) // render to shadowmap
	{
		ASSERT(light.has_component<FramebufferComponent>(),
				"Scene light doesn't have Framebuffer Component");
		ASSERT(light.has_component<CameraComponent>(),
				"Scene light doesn't have Camera Component");

		scene.light = Entity();

		SceneStatus &lsc = light.get_component<SceneStatus>();
		lsc.render = false;
		Transform &lt = light.get_component<Transform>();
		auto lc = light.get_component<CameraComponent>().camera;
		lc->position = lt.position;
		lc->update_target(model.get_component<Transform>().position);

		light.get_component<CameraComponent>().camera->aspect = 1;
		auto fb = light.get_component<FramebufferComponent>().framebuffer;
		fb->bind();
		fb->clear();
		scene.observer = light;

		if (mode == Mode::EDITOR)
			scene.on_update_editor(time_delta, editor_camera);
		else if (mode == Mode::RUNTIME)
			scene.on_update_runtime(time_delta, false);
		fb->bind_depth_texture(1);

		lsc.render = true;
	}

	scene.light = light;

    if (mode == Mode::EDITOR)
    {
        editor_camera.on_update(time_delta);

        ms_framebuffer->bind();
        ms_framebuffer->clear();

        scene.on_update_editor(time_delta, editor_camera);
        ui_scene.on_update_editor(time_delta, editor_camera);
    }
    else if (mode == Mode::RUNTIME)
    {
        ms_framebuffer->bind();
        ms_framebuffer->clear();

        scene.observer = runtime_observer;
        scene.on_update_runtime(time_delta);
    }

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

    scene_hierarchy_panel.on_imgui_render();
	observatory_panel->on_imgui_render();
}

void MainLayer::on_detach()
{
    delete ms_framebuffer;
    delete framebuffer;
	delete observatory_panel;
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
    runtime_observer.get_component<SceneStatus>().render = true;
    runtime_observer.get_component<Transform>().position =
        runtime_observer.get_component<CameraComponent>().camera->position;
}

void MainLayer::set_runtime_mode()
{
    mode = Mode::RUNTIME;
    // 		scene.observer.get_component<CameraComponent>().camera->update_target(
    // 			model.get_component<Transform>().position);
    guizmo_type = -1;
//     Application::get_window()->set_cursor_disabled();

    runtime_observer.get_component<SceneStatus>().render = false;
    runtime_observer.get_component<CameraComponent>().camera->position =
        runtime_observer.get_component<Transform>().position;
    runtime_observer.get_component<CameraComponent>().camera->update_target(
        model.get_component<Transform>().position);
    // 				vec3(0.));
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
            // 			if (ImGui::MenuItem("Save scene"))
            // 			{
            // 				SceneSerializer scene_serializer(scene);
            // 				string filename =
            // FileDialog::save_file("*.scene");
            // 				scene_serializer.serialize(filename);
            // 				cout << "saving scene" << endl;
            // 			}
            //
            // 			if (ImGui::MenuItem("Load scene"))
            // 			{
            // 				SceneSerializer scene_serializer(scene);
            // 				string filename =
            // FileDialog::open_file("*.scene");
            // 				scene_serializer.deserialize(filename);
            // 				cout << "loading scene" << endl;
            // 			}

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



        ImGui::EndMainMenuBar();
    }
    ImGui::PopFont();
}

void MainLayer::scene_window()
{
    //     if (!(scene.flags & RENDER_TO_FRAMEBUFFER))
    //         return;

// 	ImGui::Begin("depth map");
//
// 	long int shadow_tex_id = light.get_component<FramebufferComponent>().framebuffer->get_depth_attachment_id();
//     ImGui::Image((void *)shadow_tex_id, ImVec2(300, 300), ImVec2(0, 1),
//                  ImVec2(1, 0));
// 	ImGui::End();

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


