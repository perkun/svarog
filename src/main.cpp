#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

// #include "Camera.h"
// #include "Shader.h"
// #include "Texture.h"
// #include "Transform.h"
// // #include "vendor/entt/entt.hpp"
// #include "Scene/Scene.h"
// #include "Scene/SceneStatus.h"
// #include "Layer.h"

#include "Application.h"
#include "ExampleLayer.h"
#include "ImGuiLayer.h"


using namespace std;


// void set_keybindings(Application &app);


#define WIN_W 800
#define WIN_H 600


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main()
{
    cout << "This is Application!" << endl;

//     Application app;
	Application::init(WIN_W, WIN_H, "Application", false);
// 	app.window->set_cursor_normal();
	Application::get_window()->set_cursor_normal();

//     set_keybindings(app);

	Application::push_layer(new ExampleLayer);

	// RENDER LOOP
	Application::rendering_loop(GlfwEventMethod::WAIT);
	Application::destroy();
    return 0;
}

// void set_keybindings(Application &app)
// {
//     //
//     // Key mappings :)
//     app.key_pressed_map[GLFW_KEY_W] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_forwards = true;
//         //
//     };
//     app.key_pressed_map[GLFW_KEY_S] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_backwards = true;
//     };
//     app.key_pressed_map[GLFW_KEY_A] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_left = true;
//     };
//     app.key_pressed_map[GLFW_KEY_D] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_right = true;
//     };
// //
//     //
//     // KEY RELEASED
//     app.key_released_map[GLFW_KEY_W] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_forwards = false;
//         //
//     };
//     app.key_released_map[GLFW_KEY_S] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_backwards = false;
//     };
//     app.key_released_map[GLFW_KEY_A] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_left = false;
//     };
//     app.key_released_map[GLFW_KEY_D] = [](Application *app) {
//         app->active_scene->get_active_camera()->is_moving_right = false;
//     };
//
//     app.mouse_cursor_action = [](Application *app, vec2 cursor_shift) {
//         // free floating camera movement
//         // 		Camera *camera = app->active_scene->get_active_camera();
//         // 		camera->pitch(cursor_shift.y);
//         // 		camera->yaw(cursor_shift.x);
//
//         // rotating meshes
//         Transform *trans = app->active_scene->get_active_drawable_transform();
//         if (trans == NULL)
//             return;
//
//         if (trans->change_alpha)
//             trans->alpha += cursor_shift.x / 200.;
//         if (trans->change_beta)
//             trans->beta += cursor_shift.y / 300.;
//         if (trans->change_gamma)
//             trans->gamma += cursor_shift.x / 300.;
//     };
//     //
//     app.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](Application *app) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_alpha = true;
// 	};
// //
//     app.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](Application *app) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_beta = true;
// 	};
// //
//     app.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](Application *app) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_gamma = true;
// 	};
// //
//     app.mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](Application *app) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_alpha = false;
// 	};
// //
//     app.mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](Application *app) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_beta = false;
// 	};
// //
//     app.mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](Application *app) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->change_gamma = false;
// 	};
// //
//     app.mouse_scrolled_action = [](Application *app, vec2 offset) {
// 		Transform *trans = app->active_scene->get_active_drawable_transform();
// 		if (trans == NULL)
// 			return;
// 		trans->scale += offset.y / 50.f * vec3(1.0);
// 	};
// }
