#include "ExampleLayer.h"


ExampleLayer::ExampleLayer()
{
}


ExampleLayer::~ExampleLayer()
{
	delete active_scene;
// 	plane_mesh->destroy();
// 	tex_1->destroy();
// 	tex_2->destroy();
// 	basic_shader->destroy();
}


void ExampleLayer::on_attach()
{
	Window *window = Application::get_window();
    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  window->width / (float)window->height, 0.1, 100.0);

    camera.speed = 1.;

    Shader basic_shader("../src/shaders/basic_shader.vs",
						"../src/shaders/basic_shader.fs");

	// Meshes
	Mesh metis_mesh("../data/model.obj", IndexModel::PER_VERTEX);
	Mesh plane_mesh("../data/plane.obj", IndexModel::PER_VERTEX);

	// Textures
	Texture tex_1("../data/cyber.png");
	Texture tex_2("../data/horsehead.jpeg");

	// Create scene
	active_scene = new Scene();

	// Entities
	scene_camera = active_scene->create_entity();
	scene_camera.add_component<Camera>(camera);
	scene_camera.add_component<SceneStatus>(SceneStatus(true));


	plane = active_scene->create_entity();
	plane.add_component<Mesh>(plane_mesh);
	plane.add_component<Texture>(tex_1);
	plane.add_component<Shader>(basic_shader);

	Transform &tr = plane.get_component<Transform>();
	tr.beta = 1;
	tr.position = vec3(0.0, 0., 0.);

	metis = active_scene->create_entity("Metis ent");
	metis.add_component<Mesh>(metis_mesh);
	metis.add_component<Texture>(tex_2);
	metis.add_component<Shader>(basic_shader);
	metis.add_component<SceneStatus>(true);
	Transform &metis_trans = metis.get_component<Transform>();
	metis_trans.position = vec3(0.0, 0.0, 0.0);

	Transform &rt = active_scene->root_entity.get_component<Transform>();
	rt.position = vec3(-1., 0., 0.);


	active_scene->root_entity.add_child(&plane);
	plane.add_child(&metis);

}

void ExampleLayer::on_detach()
{

}



void ExampleLayer::on_event(Event &event)
{
	EventDispacher dispatcher(event);

	dispatcher.dispatch<WindowResizeEvent>(
		bind(&ExampleLayer::on_window_resize_event, this, placeholders::_1));

}

void ExampleLayer::on_window_resize_event(WindowResizeEvent &event)
{
	ivec2 size = event.get_size();
	active_scene->on_resize(size.x, size.y);

}

void ExampleLayer::on_update(double ts)
{
	if (active_scene == NULL)
	{
		cout << "No active Scene set, not rendering" << endl;
		return;
	}

	if (active_scene->get_active_camera() == NULL)
	{
		cout << "No active Camera set, not rendering" << endl;
		return;
	}

	active_scene->draw_root();
	active_scene->get_active_camera()->move(ts);

}


void ExampleLayer::on_imgui_render()
{

// 	ImGui::Begin("Settings");
// 	ImGui::ColorEdit3("Square Color", tmp);
// 	ImGui::End();

	bool show = true;
	ImGui::ShowDemoWindow(&show);


//     ImGuiIO& io = ImGui::GetIO();
//
//     ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
//
//     static int corner = 0;
//     ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
//     if (ImGui::Begin("Example: Simple overlay", &show, window_flags))
//     {
//         ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
//         ImGui::Separator();
//         if (ImGui::IsMousePosValid())
//             ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
//         else
//             ImGui::Text("Mouse Position: <invalid>");
//         if (ImGui::BeginPopupContextWindow())
//         {
//             if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
//             if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
//             if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
//             if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
//             if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
//             if (show && ImGui::MenuItem("Close")) show = false;
//             ImGui::EndPopup();
//         }
//     }
// 	ImGui::End();

}




