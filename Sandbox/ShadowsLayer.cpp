#include "ShadowsLayer.h"



ShadowsLayer::ShadowsLayer()
{
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
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
		Camera *cam = layer->active_scene->camera;
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
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		if (!cam->is_rotating)
			return;

		cam->rotate_about_target(cursor_shift);


    };

	mouse_scrolled_action = [](EventLayer *layer, vec2 offset) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;

		cam->position += cam->front * (float)(offset.y / 5. * cam->speed);
		cam->update();
	};

	key_pressed_map[GLFW_KEY_A] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_left = true;
	};

	key_released_map[GLFW_KEY_A] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_left = false;
	};

	key_pressed_map[GLFW_KEY_D] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_right = true;
	};

	key_released_map[GLFW_KEY_D] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_moving_right = false;
	};


	key_pressed_map[GLFW_KEY_W] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_forwards = true;
	};

	key_released_map[GLFW_KEY_W] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_forwards = false;
	};


	key_pressed_map[GLFW_KEY_S] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_backwards = true;
	};

	key_released_map[GLFW_KEY_S] = [](EventLayer *layer) {
		Camera *cam = layer->active_scene->camera;
		if (cam == NULL)
			return;
		cam->is_panning_backwards = false;
	};
}


ShadowsLayer::~ShadowsLayer()
{
}


void ShadowsLayer::on_attach()
{
    Window *window = Application::get_window();
    camera = new PerspectiveCamera(
        vec3(0., -30., 26.), vec3(0., 0., 0.), radians(45.0),
        window->width / (float)window->height, 0.01, 200.0);
    camera->speed = 8.;
    camera->rotation_speed = 0.002; // rads/s

    active_scene->camera = camera;

    light_camera = new OrthogonalCamera(vec3(-10., -10., 10.), vec3(0., 0., 0.),
                                        20., 1., 0.01, 50.);

    // 	#include "shaders/shadow.vs.include"
    // 	#include "shaders/shadow.fs.include"

	#include "shaders/tex_sha.vs.include"
	#include "shaders/tex_sha.fs.include"
    tex_sha_vs[tex_sha_vs_len] = 0;
    tex_sha_fs[tex_sha_fs_len] = 0;

	#include "shaders/basic_shader.vs.include"
	#include "shaders/basic_shader.fs.include"
    basic_shader_vs[basic_shader_vs_len] = 0;
    basic_shader_fs[basic_shader_fs_len] = 0;

    shader = new Shader;
    shader->create_shader((char *)((void *)tex_sha_vs),
                          (char *)((void *)tex_sha_fs));

	color_shader = new Shader;
	color_shader->create_shader((char*)((void*)basic_shader_vs), (char*)(void*)basic_shader_fs);

//     plane_texture = new ImgTexture("../../../data/dots4.png");
    plane_texture = new ImgTexture("../../../data/forest.jpg");
    asteroid_texture = new ImgTexture("../../../data/surface2.jpg");

    plane_vao =
        new VertexArrayObject(IndexedQuad(vec3(-10., -10., 0.), vec2(20.)));
    plane_vao->blend = true;

    asteroid_vao = new VertexArrayObject(
        IndexedModelObj("../../../data/model.obj", NormalIndexing::PER_FACE));

	cube_vao = new VertexArrayObject(IndexedCube());

    plane = active_scene->create_entity("Plain");
    plane.add_component<MeshComponent>(plane_vao);
    plane.add_component<TextureComponent>(plane_texture);
    plane.add_component<ShaderComponent>(shader);
    plane.add_component<SceneStatus>(false);
    plane.add_component<Material>();

    asteroid = active_scene->create_entity("Aster");
    asteroid.add_component<MeshComponent>(asteroid_vao);
    asteroid.add_component<TextureComponent>(asteroid_texture);
    asteroid.add_component<ShaderComponent>(shader);
    asteroid.add_component<SceneStatus>(true);
    asteroid.add_component<Material>();

	cube = active_scene->create_entity("Cube");
    cube.add_component<MeshComponent>(cube_vao);
    cube.add_component<ShaderComponent>(color_shader);
    cube.add_component<SceneStatus>(true);
    cube.add_component<Material>();

	Material &cmt = cube.get_component<Material>();
	cmt.uniforms_vec4["u_color"] = vec4(0.3, 0.7, 0.12, 1.);


    Transform &tr = asteroid.get_component<Transform>();
    tr.scale = vec3(5.);
    tr.position.z = 3.;

    active_scene->root_entity.add_child(&asteroid);
    active_scene->root_entity.add_child(&plane);
    active_scene->root_entity.add_child(&cube);


    // FRAMEBUFFER
    FramebufferSpecification fb_spec;
    fb_spec.width = 1024;
    fb_spec.height = 1024;

    shadow_fb = new Framebuffer(fb_spec);

	// depth texture on slot 1
	active_scene->scene_material.uniforms_int["u_depth_map"] = 1;
}

void ShadowsLayer::on_update(double time_delta)
{
	plane.get_component<Transform>().position =
		active_scene->camera->calculate_intersection_point(
			vec3(0.), vec3(0., 0., 1.));

	cube.get_component<Transform>().position = light_camera->position;

    active_scene->scene_material.uniforms_vec3["u_light_position"] =
        light_camera->position;
	active_scene->scene_material.uniforms_mat4["u_light_perspective_matrix"] =
		light_camera->get_perspective();
	active_scene->scene_material.uniforms_mat4["u_light_view_matrix"] =
		light_camera->get_view();

	light_camera->update_target(vec3(0.));



	active_scene->camera->move(time_delta);

	active_scene->camera = light_camera;
	shadow_fb->bind();
	shadow_fb->clear();
	active_scene->draw_root();
	shadow_fb->unbind();

	// TODO: get rid of opengl stuff
    glViewport(0, 0, Application::get_window()->width, Application::get_window()->height);
	shadow_fb->bind_depth_texture(1);
	active_scene->camera = camera;
	active_scene->draw_root();
	td = time_delta;
}

void ShadowsLayer::on_imgui_render()
{
    Transform &ptr = plane.get_component<Transform>();
    Transform &atr = asteroid.get_component<Transform>();

    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.3lf", 1. / td);
    ImGui::Separator();
    ImGui::Separator();

    ImGui::SliderAngle("alpha", &ptr.alpha, 0., 360.);
    ImGui::SliderAngle("beta", &ptr.beta, -90., 90.);
    ImGui::SliderAngle("gamma", &ptr.gamma, 0., 360.);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text("Asteroid");
    ImGui::SliderAngle("a alpha", &atr.alpha, 0., 360.);
    ImGui::SliderAngle("a beta", &atr.beta, -90., 90.);
    ImGui::SliderAngle("a gamma", &atr.gamma, 0., 360.);
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::SliderFloat("a x", &atr.position.x, -10., 10.);
    ImGui::SliderFloat("a y", &atr.position.y, -10., 10.);
    ImGui::SliderFloat("a z", &atr.position.z, -10., 10.);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text("Light");
    ImGui::SliderFloat("l pos x", &light_camera->position.x, -10., 10.);
    ImGui::SliderFloat("l pos y", &light_camera->position.y, -10., 10.);
    ImGui::SliderFloat("l pos z", &light_camera->position.z, -10., 10.);

    ImGui::End();


    ImGui::Begin("depth map");
//     long int tex_id = shadow_fb->get_color_attachment_id();
    long int tex_id = shadow_fb->get_depth_attachment_id();
    ImGui::Image((void *)tex_id, ImVec2(256, 256), ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();

}

void ShadowsLayer::on_detach()
{
	delete shadow_fb;

	delete camera;
	delete light_camera;

	delete plane_vao;
	delete asteroid_vao;
	delete cube_vao;

	delete plane_texture;
	delete asteroid_texture;

	delete shader;
	delete color_shader;
}

