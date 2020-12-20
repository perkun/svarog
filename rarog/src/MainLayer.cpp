#include "svpch.h"
#include "MainLayer.h"
#include "CameraController.h"
#include "ModelController.h"
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include "Math.h"

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
    vec3 init_model_pos(0., 5., 0.);

    if (arg_handler.isSpecified("model"))
	{
		string filename = arg_handler.args["model"].to_str();
		if (filename.compare(filename.rfind("."), 4, ".obj") == 0)
			model_vao = new VertexArrayObject(IndexedModelObj(
				filename, NormalIndexing::PER_FACE));
		else if (filename.compare(filename.rfind("."), 4, ".shp") == 0)
			model_vao = new VertexArrayObject(IndexedModelShp(
				filename, NormalIndexing::PER_FACE));
	}
    else
        model_vao = new VertexArrayObject(IndexedCube(vec3(-0.5), vec3(1.)));


	#include "../shaders/basic.vs.include"
	#include "../shaders/basic.fs.include"
    basic_vs[basic_vs_len] = 0;
    basic_fs[basic_fs_len] = 0;

	#include "../shaders/color.fs.include"
    color_fs[color_fs_len] = 0;

	#include "../shaders/vert_col.vs.include"
	#include "../shaders/vert_col.fs.include"
    vert_col_vs[vert_col_vs_len] = 0;
    vert_col_fs[vert_col_fs_len] = 0;

    Shader *basic_shader = new Shader();
    basic_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)basic_fs);

    Shader *color_shader = new Shader();
    color_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)color_fs);

    Shader *line_shader = new Shader();
    line_shader->create_shader((char *)(void *)vert_col_vs,
                               (char *)(void *)vert_col_fs);

	scene.shaders["basic_shader"] = basic_shader;
	scene.shaders["color_shader"] = color_shader;
	scene.shaders["line_shader"] = line_shader;


    auto window = Application::get_window();



	grid = scene.create_entity("grid");
	grid.add_component<Material>(line_shader);
	grid.add_component<MeshComponent>(new VertexArrayObject(
		create_grid(100., 5., 0.2), true));
// 	grid.get_component<SceneGraphComponent>().parent = scene.root_entity;

    scene.observer = scene.create_entity("Observer");
    CameraComponent &socp =
		scene.observer.add_component<CameraComponent>(new PerspectiveCamera(
        	radians(45.0), window->width / (float)window->height, 0.01, 500.0));
    scene.observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();
    socp.camera->update_target(init_model_pos);

    Transform &sot = scene.observer.get_component<Transform>();
    sot.position = vec3(6.3, -3., 5.12);
    sot.speed = 8.;

    scene.light = scene.create_entity("Light");
    scene.light.add_component<MeshComponent>(
        new VertexArrayObject(IndexedIcoSphere(vec3(0.), vec3(0.5))));
    scene.light.add_component<Material>(color_shader)
        .uniforms_vec4["u_color"] =
        vec4(245. / 256, 144. / 256, 17. / 256, 1.0);
// 	scene.light.get_component<SceneGraphComponent>().parent = scene.root_entity;

    model = scene.create_entity("Main model");
    model.add_component<Material>(basic_shader).uniforms_int["u_has_texture"] = 0;
    model.add_component<MeshComponent>(model_vao);
    model.add_component<NativeScriptComponent>().bind<ModelController>();
    model.get_component<Transform>().position = init_model_pos;

// 	model.get_component<SceneGraphComponent>().parent = scene.root_entity;

	Renderer::set_line_width(2.);

    scene.root_entity.add_child(model);
    scene.root_entity.add_child(grid);
    scene.root_entity.add_child(scene.light);

	scene.enable_render_to_framebuffer();

	editor_camera = EditorCamera(
		radians(45.), window->width / (float)window->height, 0.01, 500.0,
		vec3(6.3, -3., 5.12));

	scene_hierarchy_panel = SceneHierarchyPanel(&scene);
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
    ivec2 size = event.get_size();

	if (! (scene.flags & RENDER_TO_FRAMEBUFFER))
		scene.on_resize(size.x, size.y);
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

	editor_camera.on_update(time_delta);

	ASSERT(mode < Mode::NUM_MODES, "Wrong Mode");

	if (mode == Mode::EDITOR)
		scene.on_update_editor(time_delta, editor_camera);
	else if (mode == Mode::RUNTIME)
    	scene.on_update_runtime(time_delta);
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
}

void MainLayer::on_detach()
{
//     delete basic_shader;
//     delete color_shader;
//     delete line_shader;

//     if (texture != NULL)
//         delete texture;
}


void MainLayer::toggle_mode()
{
	if (mode == Mode::EDITOR)
	{
		mode = Mode::RUNTIME;
		scene.observer.get_component<CameraComponent>().camera->update_target(
			model.get_component<Transform>().position);
		guizmo_type = -1;
		Application::get_window()->set_cursor_disabled();
	}
	else if (mode == Mode::RUNTIME)
	{
		mode = Mode::EDITOR;
		Application::get_window()->set_cursor_normal();
	}

}




void MainLayer::load_model()
{
    string filename = FileDialog::open_file("*.obj *.shp");

	if (filename.compare(filename.rfind("."), 4, ".obj") == 0)
	{
		INFO("Loading OBJ model {}", filename);
		delete model_vao;

		model_vao = new VertexArrayObject(
				IndexedModelObj(filename, NormalIndexing::PER_FACE));
	}
	else if (filename.compare(filename.rfind("."), 4, ".shp") == 0)
	{
		INFO("Loading SHP model {}", filename);
		delete model_vao;

		model_vao = new VertexArrayObject(
				IndexedModelShp(filename, NormalIndexing::PER_FACE));
	}
	else
	{
		WARN("Wrong model file extension");
		return;
	}

    model.replace_component<MeshComponent>(model_vao);
}

void MainLayer::load_texture()
{
    string filename = FileDialog::open_file("*.jpg *.png *.jpeg");
    INFO("Loading texture {}", filename);
    if (texture != NULL)
        delete texture;
    texture = new ImgTexture(filename);

    if (model.has_component<TextureComponent>())
        model.replace_component<TextureComponent>(texture);
    else
        model.add_component<TextureComponent>(texture);

    model.get_component<Material>().uniforms_int["u_has_texture"] = 1;
}

void MainLayer::remove_texture()
{
    model.get_component<Material>().uniforms_int["u_has_texture"] = 0;
    if (texture != NULL)
        delete texture;
    texture = NULL;
	model.remove_component<TextureComponent>();
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

            if (ImGui::MenuItem("Load model"))
                load_model();

            if (ImGui::MenuItem("Load texture"))
                load_texture();

            if (ImGui::MenuItem("Remove texture"))
                remove_texture();

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

		string run_btn_label;
		if (mode == Mode::EDITOR)
			run_btn_label = "Run";
		else if (mode == Mode::RUNTIME)
			run_btn_label = "Stop";

		if (ImGui::Button(run_btn_label.c_str()))
		{
			toggle_mode();
		}

        ImGui::EndMainMenuBar();
    }
	ImGui::PopFont();
}

void MainLayer::scene_window()
{
    if (!(scene.flags & RENDER_TO_FRAMEBUFFER))
        return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
    ImGui::Begin("Scene");

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
    {
        viewport_panel_size.x = vps.x;
        viewport_panel_size.y = vps.y;
        scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
        editor_camera.on_resize(viewport_panel_size.x, viewport_panel_size.y);
    }

    long int tex_id = scene.framebuffer->get_color_attachment_id();
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
            (ImGuizmo::OPERATION)guizmo_type, (ImGuizmo::MODE)manipulation_ref_frame,
            glm::value_ptr(transform), glm::value_ptr(delta_matrix),
			snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
			Entity &parent = selected_entity.get_parent();
// 			if (!parent)  // no parent, root
			Transform &ptc = parent.get_component<Transform>();

            glm::vec3 translation, rotation, scale;
            glm::vec3 delta_translation, delta_rotation, delta_scale;

            Math::decompose_transform(delta_matrix,
				delta_translation, delta_rotation, delta_scale);

            Math::decompose_transform(transform, translation, rotation, scale);

			if (guizmo_type == ImGuizmo::OPERATION::TRANSLATE)
			{
				vec4 pos = vec4(tc.position, 1.0);
				pos += glm::inverse(ptc.get_world_tansform())
						* vec4(delta_translation, 0.0);
				tc.position = vec3(pos);
			}
			else if (guizmo_type == ImGuizmo::OPERATION::ROTATE)
			{
				vec4 rot = vec4(tc.rotation, 1.0);
				rot += glm::inverse(ptc.get_world_tansform())
						* vec4(delta_rotation, 0.0);
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
			scene.root_entity.add_child(grid);
	}

    ImGui::End();
}


void MainLayer::orbital_parameters_panel()
{
    Transform &t = model.get_component<Transform>();
    vec3 position = t.position;
    position /= 10.0;
    ImGui::Begin("Orbital Parameters");

//     ImGui::Text("FPS: %.3lf", fps);

    ImGui::Text("Model Position");
    if (ImGui::InputFloat("pos x", &position.x))
        t.position.x = position.x * 10;
    if (ImGui::InputFloat("pos y", &position.y))
        t.position.y = position.y * 10;
    if (ImGui::InputFloat("pos z", &position.z))
        t.position.z = position.z * 10;

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    Transform &ot = scene.observer.get_component<Transform>();
    position = ot.position;
    position /= 10.0;
    ImGui::Text("Observer Position");
    if (ImGui::InputFloat("obs pos x", &position.x))
        ot.position.x = position.x * 10;
    if (ImGui::InputFloat("obs pos y", &position.y))
        ot.position.y = position.y * 10;
    if (ImGui::InputFloat("obs pos z", &position.z))
        ot.position.z = position.z * 10;

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Angles");
    float x = t.rotation.x / M_PI * 180., y = t.rotation.y / M_PI * 180.,
          z = t.rotation.z / M_PI * 180.;

    if (ImGui::DragFloat("rot x", &x, 1.0, 0., 360.))
        t.rotation.x = x / 180. * M_PI;
    if (ImGui::DragFloat("rot y", &y, 1.0, 0., 180.))
        t.rotation.y = y / 180. * M_PI;
    if (ImGui::DragFloat("rot z", &z, 1.0, 0., 360.))
        t.rotation.z = z / 180. * M_PI;

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Scale");

    float scale = model.get_component<Transform>().scale.x;
    if (ImGui::DragFloat("scale", &scale, 0.01, 0.01, 20.))
        model.get_component<Transform>().scale = vec3(scale);

    ImGui::End();
}


IndexedModel MainLayer::create_grid(float size, float sep, float alpha)
{
	Batch grid_batch;

	for (float i = -size; i <= size; i += sep)
	{
		if (i == 0.) continue;

		grid_batch.push_back(
			IndexedLine(vec3(-size, i, 0.),
						vec3(size, i, 0.),
						vec4(0.3, 0.3, 0.3, alpha) ) );

		grid_batch.push_back(
			IndexedLine(vec3(i, -size, 0.),
						vec3(i, size, 0.),
						vec4(0.3, 0.3, 0.3, alpha) ) );
	}

	grid_batch.push_back(IndexedLine(vec3(-size, 0., 0.), vec3(size, 0., 0.),
			vec4(245./256, 74./256, 29./256, alpha)));
	grid_batch.push_back(IndexedLine(vec3(0., -size, 0.), vec3(0., size, 0.),
			vec4(28./256, 157./256, 51./256, alpha)));

	return grid_batch.indexed_model;
}
