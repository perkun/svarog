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

	cube_vao = new VertexArrayObject(IndexedCube(vec3(-0.5, -0.5, -0.5), vec3(1.)));

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
	model.add_component<MeshComponent>(cube_vao);
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



	scene_window();
}


void MainLayer::on_detach()
{
	delete basic_shader;
	delete cube_vao;
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
