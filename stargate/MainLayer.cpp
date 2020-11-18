#include "MainLayer.h"


MainLayer::MainLayer()
{
	key_pressed_map[GLFW_KEY_A] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_left = true;
	};

	key_released_map[GLFW_KEY_A] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_left = false;
	};

	key_pressed_map[GLFW_KEY_D] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_right = true;
	};

	key_released_map[GLFW_KEY_D] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_right = false;
	};


	key_pressed_map[GLFW_KEY_W] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_up = true;
	};

	key_released_map[GLFW_KEY_W] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_up = false;
	};


	key_pressed_map[GLFW_KEY_S] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_down = true;
	};

	key_released_map[GLFW_KEY_S] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_down = false;
	};

}


MainLayer::~MainLayer()
{

}


void MainLayer::on_attach()
{
    Window *window = Application::get_window();

//     active_scene->camera = new PerspectiveCamera(
//         vec3(0., 0., 10.), vec3(0., 0., 0.), radians(45.0),
//         window->width / (float)window->height, 0.01, 20.0);

    active_scene->camera = new OrthogonalCamera(
        vec3(0., 0., 10.), vec3(0., 0., 0.), 10.,
        window->width / (float)window->height, 0.01, 20.0);
    active_scene->camera->speed = 8.;
    active_scene->camera->rotation_speed = 0.002; // rads/s


	#include "shaders/texture_shader.vs.include"
	#include "shaders/texture_shader.fs.include"
	texture_shader_vs[texture_shader_vs_len] = 0;
	texture_shader_fs[texture_shader_fs_len] = 0;

	shader = new Shader;
	shader->create_shader((char*)((void*)texture_shader_vs),
						  (char*)((void*)texture_shader_fs));

// 	input_image = new ImgTexture("../../../data/makeeva2.png");
// 	fits_texture = new FitsTexture("../../../data/zyla_rolling.fits");
// 	fits_texture = new FitsTexture("../../../data/julia2.fits");
	fits_texture = new FitsTexture("../../../data/pst2_3.fits");

	vec2 plane_scale = fits_texture->get_dimentions();
	plane_scale /= plane_scale.y; // normalize
	plane_scale *= 4.;

	plane_vao = new VertexArrayObject(
						IndexedQuad(vec3(-2.5, -2.5, 0.), plane_scale)
					);

	plane = active_scene->create_entity("palne");
	plane.add_component<MeshComponent>(plane_vao);
	plane.add_component<TextureComponent>(fits_texture);
    plane.add_component<Material>();
	plane.add_component<ShaderComponent>(shader);
    plane.add_component<SceneStatus>(true);

	active_scene->root_entity.add_child(&plane);

	init_min = fits_texture->min;
	init_max = fits_texture->max;


// 	fits_texture->multiply_data(brightness);
// 	fits_texture->update_texture();
}


void MainLayer::on_update(double time_delta)
{
	active_scene->camera->move(time_delta);
	active_scene->draw_root();
}

void MainLayer::on_imgui_render()
{
	Camera *cam = active_scene->camera;

	ImGui::Begin("FPS");
//     ImGui::SliderFloat("pos x", &(cam->position.x), -5, 5, "%.3f");
//     ImGui::SliderFloat("pos y", &(cam->position.y), -5, 5, "%.3f");
//     ImGui::SliderFloat("pos z", &(cam->position.z), -5, 5, "%.3f");


	ImGui::DragFloat("min", &fits_texture->min, 0., init_max);
	ImGui::DragFloat("max", &fits_texture->max, init_min, init_max);

	bool clicked = false;
	if (ImGui::Button("Update"))
		clicked = true;

	if (clicked)
	{
		fits_texture->normalize_data();
		fits_texture->update_texture();
	}

	ImGui::End();

// 	ImGui::ShowDemoWindow();

	cam->update();
}

void MainLayer::on_detach()
{
	delete plane_vao;
// 	delete input_image;
	delete fits_texture;
	delete shader;
}

