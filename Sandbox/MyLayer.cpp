#include "MyLayer.h"
#include "Transform.h"


MyLayer::MyLayer()
{
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_alpha = true;
	};
//
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](EventLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_beta = true;
	};
//
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](EventLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_gamma = true;
	};

    mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](EventLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_alpha = false;
	};
//
    mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](EventLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_beta = false;
	};
//
    mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](EventLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_gamma = false;
	};


    mouse_cursor_action = [](EventLayer *layer, vec2 cursor_shift) {
        // free floating camera movement
        // 		Camera *camera = app->active_scene->get_active_camera();
        // 		camera->pitch(cursor_shift.y);
        // 		camera->yaw(cursor_shift.x);
//
        // rotating meshes
        Transform *trans = layer->active_scene->get_active_drawable_transform();
        if (trans == NULL)
            return;
//
        if (trans->change_alpha)
            trans->alpha += cursor_shift.x / 200.;
        if (trans->change_beta)
            trans->beta += cursor_shift.y / 300.;
        if (trans->change_gamma)
            trans->gamma += cursor_shift.x / 300.;
    };
}


MyLayer::~MyLayer()
{
}

void MyLayer::on_attach()
{
    Window *window = Application::get_window();
    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  window->width / (float)window->height, 0.01, 20.0);
    camera.speed = 1.;

//     Shader color_shader("../../../Sandbox/shaders/color_shader.vs",
//                         "../../../Sandbox/shaders/color_shader.fs");

	#include "shaders/color_shader.vs.include"
	#include "shaders/color_shader.fs.include"
	color_shader_vs[color_shader_vs_len] = 0;
	color_shader_fs[color_shader_fs_len] = 0;

	Shader color_shader;
	color_shader.create_shader((char*)((void*)color_shader_vs),
						       (char*)((void*)color_shader_fs));

	#include "shaders/texture_shader.vs.include"
	#include "shaders/texture_shader.fs.include"
	texture_shader_vs[texture_shader_vs_len] = 0;
	texture_shader_fs[texture_shader_fs_len] = 0;
//
	Shader texture_shader;
	texture_shader.create_shader((char*)((void*)texture_shader_vs),
						       (char*)((void*)texture_shader_fs));

    Mesh model_mesh(model_filename, IndexModel::PER_VERTEX);
	Mesh plane_mesh(plane_filename, IndexModel::PER_FACE);
	plane_mesh.blend = true;

	Texture texture(texture_filename);


    model = active_scene->create_entity("Model");
    model.add_component<Mesh>(model_mesh);
    model.add_component<SceneStatus>(true);
    model.add_component<Material>();
    model.add_component<Shader>(color_shader);
//     model.add_component<Texture>(texture);

    Material &model_material = model.get_component<Material>();
    model_material.uniforms_vec4["u_color"] = vec4(1.0);


    plane = active_scene->create_entity("Plane");
    plane.add_component<Mesh>(plane_mesh);
    plane.add_component<SceneStatus>(false);
    plane.add_component<Material>();
    plane.add_component<Texture>(texture);
    plane.add_component<Shader>(texture_shader);
//
	Transform &pt = plane.get_component<Transform>();
	pt.beta = 0.37;
	pt.gamma = 0.3;
	pt.position.x = 0.47;
	pt.position.y = 3.75;
	pt.position.z = -0.3;
	pt.scale = vec3(5.);
    Material &plane_material = model.get_component<Material>();
    plane_material.uniforms_vec4["u_color"] = vec4(1.0);



    active_scene->scene_material.uniforms_vec3["u_light_direction"] =
        vec3(0.0, -1.0, 0.0);


    scene_camera = active_scene->create_entity();
    scene_camera.add_component<Camera>(camera);
    scene_camera.add_component<SceneStatus>(true);

    // 	active_scene->root_entity.add_child(&scene_camera);
    active_scene->root_entity.add_child(&model);
    active_scene->root_entity.add_child(&plane);
}

void MyLayer::on_detach()
{

}

void MyLayer::on_update(double time_delta)
{

	active_scene->draw_root();

}

void MyLayer::on_imgui_render()
{
    ImGui::SetNextWindowBgAlpha(0.0);
    ImGui::SetNextWindowPos(ImVec2(0., 0.));
    ImGui::SetNextWindowSize(ImVec2(300, 550));

    ImGui::Begin("Settings", NULL,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Light direction");
    Material &sm = active_scene->scene_material;
    ImGui::SliderFloat("light pos x",
                       &(sm.uniforms_vec3["u_light_direction"].x), -1.0, 1.0,
                       "%.3f");
    ImGui::SliderFloat("lightpos y", &(sm.uniforms_vec3["u_light_direction"].y),
                       -1.0, 1.0, "%.3f");
    ImGui::SliderFloat("lightpos z", &(sm.uniforms_vec3["u_light_direction"].z),
                       -1.0, 1.0, "%.3f");


    ImGui::Text("plane pos");
    Transform &pt = plane.get_component<Transform>();
    ImGui::SliderFloat("alpha", &(pt.alpha), 0., 2*M_PI, "%.3f");
    ImGui::SliderFloat("beta", &(pt.beta), -M_PI/2., M_PI/2., "%.3f");
    ImGui::SliderFloat("gamma", &(pt.gamma), 0., 2*M_PI, "%.3f");
    ImGui::SliderFloat("pos x", &(pt.position.x), -5, 5, "%.3f");
    ImGui::SliderFloat("pos y", &(pt.position.y), -5, 5, "%.3f");
    ImGui::SliderFloat("pos z", &(pt.position.z), -5, 5, "%.3f");



    ImGui::End();
}
