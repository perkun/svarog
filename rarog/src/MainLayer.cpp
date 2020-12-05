#include "MainLayer.h"

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

    basic_shader = new Shader();
    basic_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)basic_fs);

    color_shader = new Shader();
    color_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)color_fs);

    line_shader = new Shader();
    line_shader->create_shader((char *)(void *)vert_col_vs,
                               (char *)(void *)vert_col_fs);
    if (arg_handler.isSpecified("model"))
        model_vao = new VertexArrayObject(IndexedModelObj(
            arg_handler.args["model"].to_str(), NormalIndexing::PER_FACE));
    else
        model_vao = new VertexArrayObject(IndexedCube(vec3(-0.5), vec3(1.)));

    auto window = Application::get_window();

    scene->observer = scene->create_entity("Observer");
    scene->observer.add_component<CameraComponent>(new PerspectiveCamera(
        radians(45.0), window->width / (float)window->height, 0.01, 500.0));
    scene->observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();
    Transform &sot = scene->observer.get_component<Transform>();
    sot.position = vec3(6.3, -3., 5.12);
    sot.update_target(init_model_pos);
    sot.speed = 8.;

    scene->light = scene->create_entity("Light");
    scene->light.add_component<MeshComponent>(
        new VertexArrayObject(IndexedIcoSphere(vec3(0.), vec3(0.5))));
    scene->light.add_component<Material>(color_shader)
        .uniforms_vec4["u_color"] =
        vec4(245. / 256, 144. / 256, 17. / 256, 1.0);

    model = scene->create_entity("Main model");
    model.add_component<Material>(basic_shader).uniforms_int["u_has_texture"] = 0;
    model.add_component<MeshComponent>(model_vao);
    model.add_component<NativeScriptComponent>().bind<ModelController>();
    model.get_component<Transform>().position = init_model_pos;


	Batch grid_batch;
	for (float i = -50; i <= 50; i += 5)
	{
		if (i == 0) continue;
		grid_batch.push_back(
			IndexedLine(vec3(-50., i, 0.), vec3(50., i, 0.),
				vec4(0.3, 0.3, 0.3, 0.5)));
		grid_batch.push_back(
			IndexedLine(vec3(i, -50., 0.), vec3(i, 50., 0.),
				vec4(0.3, 0.3, 0.3, 0.5)));
	}
	grid_batch.push_back(IndexedLine(vec3(-50., 0., 0.), vec3(50., 0., 0.),
			vec4(245./256, 74./256, 29./256, 0.5)));
	grid_batch.push_back(IndexedLine(vec3(0., -50., 0.), vec3(0., 50., 0.),
			vec4(28./256, 157./256, 51./256, 0.5)));

	grid = scene->create_entity("grid");
	grid.add_component<Material>(line_shader);
	grid.add_component<MeshComponent>(new VertexArrayObject(grid_batch.indexed_model, true));
// 	grid.add_component<MeshComponent>(new VertexArrayObject(
// 					IndexedLine(vec3(10., -50., 0.), vec3(10., 50., 0.),
// 					vec4(0.3, 0.3, 0.3, 0.5)), true));


	Renderer::set_line_width(2.);

    scene->root_entity.add_child(&model);
    scene->root_entity.add_child(&grid);
    scene->root_entity.add_child(&scene->light);


	scene->enable_render_to_framebuffer();
}

void MainLayer::on_update(double time_delta)
{
    scene->on_update(time_delta);

}

void MainLayer::on_imgui_render()
{
    ImGui::DockSpaceOverViewport();
    menu_bar();
    scene_window();
    orbital_parameters_panel();

	if (show_scene_options)
		scene_options();

    if (show_imgui_demo)
        ImGui::ShowDemoWindow();
}

void MainLayer::on_detach()
{
    delete basic_shader;
    delete color_shader;
    delete line_shader;

    if (texture != NULL)
        delete texture;
}

void MainLayer::load_model()
{
    string filename = FileDialog::open_file("*.obj *.shp");

	size_t dot_pos = filename.rfind(".");
	TRACE("dot_pos {}", dot_pos);
	if (filename.compare(dot_pos, 4, ".obj") == 0)
	{
		INFO("Loading OBJ model {}", filename);
		delete model_vao;

		model_vao = new VertexArrayObject(
				IndexedModelObj(filename, NormalIndexing::PER_FACE));
	}
	else if (filename.compare(dot_pos, 4, ".shp") == 0)
	{
		INFO("Loading SHP model {}", filename);
		delete model_vao;

		model_vao = new VertexArrayObject(
				IndexedModelShp(filename, NormalIndexing::PER_FACE));
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

        ImGui::EndMainMenuBar();
    }
	ImGui::PopFont();
}

void MainLayer::scene_window()
{
	if (!(scene->flags & RENDER_TO_FRAMEBUFFER))
		return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
    ImGui::Begin("Scene");

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
    {
        viewport_panel_size.x = vps.x;
        viewport_panel_size.y = vps.y;
        scene->on_resize(viewport_panel_size.x, viewport_panel_size.y);
    }

    long int tex_id = scene->framebuffer->get_color_attachment_id();
    ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.y), ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}


void MainLayer::scene_options()
{

    ImGui::Begin("Scene Options");
	if (ImGui::Checkbox("show grid", &show_grid))
	{
		grid.detatch();
		if (show_grid)
			scene->root_entity.add_child(&grid);
	}

    ImGui::End();
}


void MainLayer::orbital_parameters_panel()
{
    Transform &t = model.get_component<Transform>();
    vec3 position = t.position;
    position /= 10.0;
    ImGui::Begin("Orbital Parameters");

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

    Transform &ot = scene->observer.get_component<Transform>();
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
    float a = t.alpha / M_PI * 180., b = t.beta / M_PI * 180.,
          g = t.gamma / M_PI * 180.;
    if (ImGui::DragFloat("alpha", &a, 1.0, 0., 360.))
        t.alpha = a / 180. * M_PI;
    if (ImGui::DragFloat("beta", &b, 1.0, 0., 180.))
        t.beta = b / 180. * M_PI;
    if (ImGui::DragFloat("gamma", &g, 1.0, 0., 360.))
        t.gamma = g / 180. * M_PI;

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
