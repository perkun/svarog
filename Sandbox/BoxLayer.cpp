#include "BoxLayer.h"


BoxLayer::BoxLayer()
{
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_rotating = true;
	};
//
//     mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](EventLayer *layer) {
// 		Transform *trans = layer->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_beta = true;
// 	};
// //
//     mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](EventLayer *layer) {
// 		Transform *trans = layer->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_gamma = true;
// 	};

    mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_rotating = false;
	};
//
//     mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](EventLayer *layer) {
// 		Transform *trans = layer->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_beta = false;
// 	};
// //
//     mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](EventLayer *layer) {
// 		Transform *trans = layer->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_gamma = false;
// 	};


    mouse_cursor_action = [](EventLayer *layer, vec2 cursor_shift) {
        // free floating camera movement
        // 		Camera *camera = app->active_scene->get_active_camera();
        // 		camera->pitch(cursor_shift.y);
        // 		camera->yaw(cursor_shift.x);
//
        // rotating meshes
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		if (!cam->is_rotating)
			return;

		cam->rotate_about_target(cursor_shift);


    };

	mouse_scrolled_action = [](EventLayer *layer, vec2 offset) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;

		cam->position += cam->front * (float)(offset.y / 5. * cam->speed);
		cam->update();
	};

	key_pressed_map[GLFW_KEY_A] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_moving_left = true;
	};

	key_released_map[GLFW_KEY_A] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_moving_left = false;
	};

	key_pressed_map[GLFW_KEY_D] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_moving_right = true;
	};

	key_released_map[GLFW_KEY_D] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_moving_right = false;
	};


	key_pressed_map[GLFW_KEY_W] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_panning_forwards = true;
	};

	key_released_map[GLFW_KEY_W] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_panning_forwards = false;
	};


	key_pressed_map[GLFW_KEY_S] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_panning_backwards = true;
	};

	key_released_map[GLFW_KEY_S] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->get_active_camera();
		if (cam == NULL)
			return;
		cam->is_panning_backwards = false;
	};
}


BoxLayer::~BoxLayer()
{
}


void BoxLayer::on_attach()
{
    Window *window = Application::get_window();
    Camera camera(vec3(0., -30., 26.), vec3(0., 0., 0.), radians(45.0),
                  window->width / (float)window->height, 0.01, 200.0);
    camera.speed = 8.;
	camera.rotation_speed = 0.002;  // rads/s

    scene_camera = active_scene->create_entity();
    scene_camera.add_component<Camera>(camera);
    scene_camera.add_component<SceneStatus>(true);


	#include "shaders/color_shader.vs.include"
	#include "shaders/explode.gs.include"
	#include "shaders/color_shader.fs.include"
	color_shader_vs[color_shader_vs_len] = 0;
	explode_gs[explode_gs_len] = 0;
	color_shader_fs[color_shader_fs_len] = 0;


	color_shader = new Shader;
	color_shader->create_shader((char*)(void*)color_shader_vs,
							   (char*)(void*)explode_gs,
							   (char*)(void*)color_shader_fs);

	#include "shaders/texture_shader.vs.include"
	#include "shaders/texture_shader.fs.include"
	texture_shader_vs[texture_shader_vs_len] = 0;
	texture_shader_fs[texture_shader_fs_len] = 0;
//
	texture_shader = new Shader;
	texture_shader->create_shader((char*)((void*)texture_shader_vs),
						       (char*)((void*)texture_shader_fs));



	plane_vao = new VertexArrayObject(IndexedQuad(vec3(-12, -12, 0), vec2(24)));
	plane_vao->blend = true;

	texture = new Texture("../../../data/dots4.png");
// 	Texture tex2("../../../data/makeeva2.png");

    plane = active_scene->create_entity("Plane");
    plane.add_component<MeshComponent>(plane_vao);
    plane.add_component<SceneStatus>(false);
    plane.add_component<Material>();
    plane.add_component<TextureComponent>(texture);
    plane.add_component<ShaderComponent>(texture_shader);

	Batch cube_batch;
// 	IndexedCube indexed_cube(vec3(0, 0, 0), vec3(0.7));
// 	cube_batch.models.push_back(indexed_cube);

	srand(12313);
	// CUBE GENERATION
	int s = 1;
	for (int k = 0; k < s; k += 1)
	for (int i = 0; i < s; i += 1)
	for (int j = 0; j < s; j += 1)
	{
// 		if (rand()%2 == 0)
// 			continue;
		cube_batch.models.push_back(
			IndexedColorCube(vec3(i-s/2, j-s/2, k), vec3(0.7),
							 vec4(
							 	normalize(vec3((rand()%100)/100.,
							 	(rand()%100)/100.,
							 	(rand()%100)/100.)),
							 	(rand()%100)/100. )
							)
		);
	}

	cube_batch.make_batch();

	cube_vao = new VertexArrayObject(cube_batch.batch);
	cube_vao->blend = true;

	cube = active_scene->create_entity("CUBE");
	cube.add_component<MeshComponent>(cube_vao);
	cube.add_component<ShaderComponent>(color_shader);
	cube.add_component<Material>();
	cube.add_component<SceneStatus>(true);

// 	Material &cm = cube.get_component<Material>();
// 	cm.uniforms_vec4["u_color"] = vec4(0.6, 0.1, 0.5, 0.5);
												// (rand()%80)/100. + 0.2

	active_scene->root_entity.add_child(&cube);
	active_scene->root_entity.add_child(&plane);

	active_scene->scene_material
		.uniforms_vec3["u_light_direction"] = vec3(0, -1, 0);
}


void BoxLayer::on_detach()
{
	delete cube_vao;
	delete plane_vao;
	delete texture;
	delete color_shader;
	delete texture_shader;
}


void BoxLayer::on_update(double time_delta)
{
	plane.get_component<Transform>().position =
		active_scene->get_active_camera()->calculate_intersection_point(
			vec3(0.), vec3(0., 0., 1.));

	active_scene->scene_material.uniforms_float["time"] = glfwGetTime();

	active_scene->get_active_camera()->move(time_delta);
	active_scene->draw_root();
	td = time_delta;
}

void BoxLayer::on_imgui_render()
{
	ImGui::Begin("FPS");
	ImGui::Text("FPS: %.3lf", 1./td);
	ImGui::End();

}
