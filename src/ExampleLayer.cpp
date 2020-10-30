#include "ExampleLayer.h"

#define WIN_W 800
#define WIN_H 600

ExampleLayer::ExampleLayer()
{
	cout << "ExampleLayer constructor" << "\n";

    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  WIN_W / (float)WIN_H, 0.1, 100.0);

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


ExampleLayer::~ExampleLayer()
{
	delete active_scene;
// 	plane_mesh->destroy();
// 	tex_1->destroy();
// 	tex_2->destroy();
// 	basic_shader->destroy();
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

void ExampleLayer::on_attach()
{

}

void ExampleLayer::on_detach()
{

}
