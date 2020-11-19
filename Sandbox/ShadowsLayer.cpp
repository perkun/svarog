#include "ShadowsLayer.h"



ShadowsLayer::ShadowsLayer()
{
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
        layer->scene->observer.get_component<Transform>().is_rotating =
            true;
    };
    //
    //     mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](EventLayer *layer)
    //     {
    // 		Transform *trans =
    // layer->scene->get_active_drawable_transform(); 		if (trans == NULL)
    // 			return;
    // 		trans->change_beta = true;
    // 	};
    // //
    //     mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](EventLayer *layer)
    //     {
    // 		Transform *trans =
    // layer->scene->get_active_drawable_transform(); 		if (trans == NULL)
    // 			return;
    // 		trans->change_gamma = true;
    // 	};
    //
    mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
        layer->scene->observer.get_component<Transform>().is_rotating =
            false;
	};
//
//     mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](EventLayer *layer) {
// 		Transform *trans = layer->scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_beta = false;
// 	};
// //
//     mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](EventLayer *layer) {
// 		Transform *trans = layer->scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_gamma = false;
// 	};
//
//
    mouse_cursor_action = [](EventLayer *layer, vec2 cursor_shift) {
        Transform &tr = layer->scene->observer.get_component<Transform>();
		if (tr.is_rotating)
            tr.rotate_about_target(cursor_shift);
    };

    mouse_scrolled_action = [](EventLayer *layer, vec2 offset) {
//         Camera *cam = layer->scene->camera;
//         if (cam == NULL)
//             return;
//
//         cam->position += cam->front * (float)(offset.y / 5. * cam->speed);
//         cam->update();
        Transform &tr = layer->scene->observer.get_component<Transform>();
		tr.position += tr.front * (float)(offset.y / 5. * tr.speed);
    };

    // 	key_pressed_map[GLFW_KEY_A] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_moving_left = true;
    // 	};
    //
    // 	key_released_map[GLFW_KEY_A] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_moving_left = false;
    // 	};
    //
    // 	key_pressed_map[GLFW_KEY_D] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_moving_right = true;
    // 	};
    //
    // 	key_released_map[GLFW_KEY_D] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_moving_right = false;
    // 	};
    //
    //
    // 	key_pressed_map[GLFW_KEY_W] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_panning_forwards = true;
    // 	};
    //
    // 	key_released_map[GLFW_KEY_W] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_panning_forwards = false;
    // 	};
    //
    //
    // 	key_pressed_map[GLFW_KEY_S] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_panning_backwards = true;
    // 	};
    //
    // 	key_released_map[GLFW_KEY_S] = [](EventLayer *layer) {
    // 		Camera *cam = layer->scene->camera;
    // 		if (cam == NULL)
    // 			return;
    // 		cam->is_panning_backwards = false;
    // 	};
}


ShadowsLayer::~ShadowsLayer()
{
}


void ShadowsLayer::on_attach()
{
	scene = new Scene;
    Window *window = Application::get_window();
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
    color_shader->create_shader((char *)((void *)basic_shader_vs),
                                (char *)(void *)basic_shader_fs);

    plane_texture = new ImgTexture("../../../data/dots4.png");
    //     plane_texture = new ImgTexture("../../../data/forest.jpg");
    asteroid_texture = new ImgTexture("../../../data/surface2.jpg");

    plane_vao =
        new VertexArrayObject(IndexedQuad(vec3(-10., -10., 0.), vec2(20.)));
    plane_vao->blend = true;

    asteroid_vao = new VertexArrayObject(
        IndexedModelObj("../../../data/model.obj", NormalIndexing::PER_FACE));

    cube_vao = new VertexArrayObject(IndexedCube());


    scene->observer = scene->create_entity("Camera");
    scene->observer.add_component<CameraComponent>(new PerspectiveCamera(
        radians(45.0), window->width / (float)window->height, 0.01, 200.0));
	Transform &sot = scene->observer.get_component<Transform>();
	sot.position = vec3(0., -30., 26.);
    sot.update_target(vec3(0., 0., 0.));
    sot.speed = 8.;

    scene->light = scene->create_entity("Light");
    scene->light.add_component<CameraComponent>(
        new OrthogonalCamera(20., 1., 0.01, 50.));

    FramebufferSpecification fb_spec;
    fb_spec.width = 1024;
    fb_spec.height = 1024;
    scene->light.add_component<FramebufferComponent>(
        new Framebuffer(fb_spec));

    Transform &slt = scene->light.get_component<Transform>();
	slt.position = vec3(-10., -10., 10.);
    slt.update_target(vec3(0., 0., 0.));

    plane = scene->create_entity("Plain");
    plane.add_component<MeshComponent>(plane_vao);
    plane.add_component<TextureComponent>(plane_texture);
    plane.add_component<SceneStatus>(false);
    plane.add_component<Material>(shader);

    asteroid = scene->create_entity("Aster");
    asteroid.add_component<MeshComponent>(asteroid_vao);
    asteroid.add_component<TextureComponent>(asteroid_texture);
    asteroid.add_component<SceneStatus>(true);
    asteroid.add_component<Material>(shader);

    Transform &tr = asteroid.get_component<Transform>();
    tr.scale = vec3(5.);
    tr.position.z = 3.;

    cube = scene->create_entity("Cube");
    cube.add_component<MeshComponent>(cube_vao);
    cube.add_component<SceneStatus>(true);
    Material &cmt = cube.add_component<Material>(color_shader);
    cmt.uniforms_vec4["u_color"] = vec4(0.3, 0.7, 0.12, 1.);


    scene->root_entity.add_child(&asteroid);
    scene->root_entity.add_child(&plane);
    scene->root_entity.add_child(&cube);

    // depth texture on slot 1
    scene->scene_material.uniforms_int["u_depth_map"] = 1;
}

void ShadowsLayer::on_update(double time_delta)
{
    plane.get_component<Transform>().position =
        scene->observer.get_component<Transform>()
            .calculate_intersection_point(vec3(0.), vec3(0., 0., 1.));

    Transform &slt = scene->light.get_component<Transform>();
    slt.update_target(vec3(0.));
    //
    cube.get_component<Transform>().position = slt.position;

    // From Light Perspective ////////
    scene->draw_root(POV::LIGHT);

    // From normal Camera /////////
    scene->draw_root(POV::OBSERVER);
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


    ImGui::DragFloat("a x", &atr.position.x, 0.2);
    ImGui::DragFloat("a y", &atr.position.y, 0.2);
    ImGui::DragFloat("a z", &atr.position.z, 0.2);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text("Light");
	Transform &slt = scene->light.get_component<Transform>();
    ImGui::DragFloat("l pos x", &slt.position.x, 0.2);
    ImGui::DragFloat("l pos y", &slt.position.y, 0.2);
    ImGui::DragFloat("l pos z", &slt.position.z, 0.2);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text("Camera");
	Transform &sct = scene->observer.get_component<Transform>();
    ImGui::DragFloat("c pos x", &sct.position.x, 0.2);
    ImGui::DragFloat("c pos y", &sct.position.y, 0.2);
    ImGui::DragFloat("c pos z", &sct.position.z, 0.2);

    ImGui::End();


//     ImGui::Begin("depth map");
// //     long int tex_id = shadow_fb->get_color_attachment_id();
//     long int tex_id = shadow_fb->get_depth_attachment_id();
//     ImGui::Image((void *)tex_id, ImVec2(256, 256), ImVec2(0, 1),
//                  ImVec2(1, 0));
//     ImGui::End();

}

void ShadowsLayer::on_detach()
{

	delete plane_vao;
	delete asteroid_vao;
	delete cube_vao;

	delete plane_texture;
	delete asteroid_texture;

	delete shader;
	delete color_shader;
}

