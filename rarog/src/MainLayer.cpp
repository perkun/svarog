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
	#include "../shaders/basic.vs.include"
	#include "../shaders/basic.fs.include"
    basic_vs[basic_vs_len] = 0;
    basic_fs[basic_fs_len] = 0;

	basic_shader = new Shader();
	basic_shader->create_shader((char*)(void*)basic_vs, (char*)(void*)basic_fs);

	if (arg_handler.isSpecified("model"))
		model_vao = new VertexArrayObject(IndexedModelObj(
			arg_handler.args["model"].to_str(), NormalIndexing::PER_FACE));
	else
		model_vao = new VertexArrayObject(IndexedCube(vec3(-0.5, -0.5, -0.5), vec3(1.)));

	auto window = Application::get_window();

	scene->observer = scene->create_entity("Observer");
    scene->observer.add_component<CameraComponent>(new PerspectiveCamera(
        radians(45.0), window->width / (float)window->height, 0.01, 500.0));
    scene->observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();
    Transform &sot = scene->observer.get_component<Transform>();
    sot.position = vec3(0., -10., 5.);
    sot.update_target(vec3(0., 0., 0.));
    sot.speed = 8.;

// 	scene->light = scene->create_entity("Light");

	model = scene->create_entity("Main model");
	model.add_component<Material>(basic_shader);
	model.add_component<MeshComponent>(model_vao);
	model.add_component<NativeScriptComponent>().bind<ModelController>();

	scene->root_entity.add_child(&model);

	scene->framebuffer = new Framebuffer(window->width, window->height,
										 COLOR_ATTACHMENT | DEPTH_ATTACHMENT);

	scene->flags |= RENDER_TO_FRAMEBUFFER;
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

	ImGui::ShowDemoWindow();

}


void MainLayer::on_detach()
{
	delete basic_shader;
	delete model_vao;
	if (texture != NULL)
		delete texture;
}

void MainLayer::menu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
// 			if (ImGui::MenuItem("Save scene"))
// 			{
// 				SceneSerializer scene_serializer(scene);
// 				string filename = FileDialog::save_file("*.scene");
// 				scene_serializer.serialize(filename);
// 				cout << "saving scene" << endl;
// 			}
//
// 			if (ImGui::MenuItem("Load scene"))
// 			{
// 				SceneSerializer scene_serializer(scene);
// 				string filename = FileDialog::open_file("*.scene");
// 				scene_serializer.deserialize(filename);
// 				cout << "loading scene" << endl;
// 			}

			if (ImGui::MenuItem("Load model"))
			{
				string filename = FileDialog::open_file("*.obj *.shp");
				INFO("Loading model {}", filename);
				delete model_vao;
				model_vao = new VertexArrayObject(IndexedModelObj(filename,
												  NormalIndexing::PER_FACE));
				model.replace_component<MeshComponent>(model_vao);
			}

			if (ImGui::MenuItem("Load texture"))
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
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}


void MainLayer::scene_window()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
	ImGui::Begin("Scene");

	ImVec2 vps = ImGui::GetContentRegionAvail();
	if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
	{
		viewport_panel_size.x = vps.x;
		viewport_panel_size.y = vps.y;
		scene->on_resize(viewport_panel_size.x, viewport_panel_size.y);

		ASSERT(scene->framebuffer != NULL, "Framebuffer is NULL");
		scene->framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
	}

	long int tex_id = scene->framebuffer->get_color_attachment_id();
	ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

void MainLayer::orbital_parameters_panel()
{
	Transform &t = model.get_component<Transform>();
	vec3 position = t.position;
	position /= 10.0;
	ImGui::Begin("Orbital Parameters");

	ImGui::Text("Model Position");
	if (ImGui::InputFloat("pos x", &position.x)) t.position.x = position.x * 10;
	if (ImGui::InputFloat("pos y", &position.y)) t.position.y = position.y * 10;
	if (ImGui::InputFloat("pos z", &position.z)) t.position.z = position.z * 10;

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	Transform &ot = scene->observer.get_component<Transform>();
	position = ot.position;
	position /= 10.0;
	ImGui::Text("Observer Position");
	if (ImGui::InputFloat("obs pos x", &position.x)) ot.position.x = position.x * 10;
	if (ImGui::InputFloat("obs pos y", &position.y)) ot.position.y = position.y * 10;
	if (ImGui::InputFloat("obs pos z", &position.z)) ot.position.z = position.z * 10;

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text("Angles");
	float a = t.alpha / M_PI * 180., b = t.beta / M_PI * 180., g = t.gamma / M_PI * 180.;
	if (ImGui::DragFloat("alpha", &a, 1.0, 0., 360.)) t.alpha = a / 180. * M_PI;
	if (ImGui::DragFloat("beta", &b , 1.0, 0., 180.)) t.beta = b / 180. * M_PI;
	if (ImGui::DragFloat("gamma", &g, 1.0, 0., 360.)) t.gamma = g / 180. * M_PI;

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	ImGui::End();
}
