#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Application.h"
#include "Texture.h"
#include "Transform.h"
// #include "vendor/entt/entt.hpp"
#include "Scene.h"

#define WIN_W 800
#define WIN_H 600

using namespace std;


void set_keybindings(Application &app);


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main()
{
    cout << "This is Application!" << endl;

    Application app(WIN_W, WIN_H, "Application", false);
    set_keybindings(app);

    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  WIN_W / (float)WIN_H, 0.1, 100.0);

    camera.speed = 1.;

    Shader basic_shader("../src/shaders/basic_shader.vs",
						"../src/shaders/basic_shader.fs");

	// Meshes
	Mesh metis_mesh("../data/model.obj", IndexModel::PER_VERTEX);
	Mesh plane_mesh("../data/plane.obj", IndexModel::PER_VERTEX);

	// Textures
	Texture tex_1("../data/makeeva2.png");
	Texture tex_2("../data/forest.jpg");

	// Create scene
	Scene scene(camera);

	app.active_scene = &scene;

	// Entities
	Entity plane = scene.create_entity();
	plane.add_component<Mesh>(plane_mesh);
	plane.add_component<Texture>(tex_1);
	plane.add_component<Shader>(basic_shader);

	Transform &tr = plane.get_component<Transform>();
	tr.beta = 1;
	tr.position = vec3(0.0, 0., 0.);

	Entity metis = scene.create_entity("Metis ent");
	metis.add_component<Mesh>(metis_mesh);
	metis.add_component<Texture>(tex_2);
	metis.add_component<Shader>(basic_shader);
	Transform &metis_trans = metis.get_component<Transform>();
	metis_trans.position = vec3(0.0, 0.0, 0.0);


	Transform &rt = scene.root_entity.get_component<Transform>();
	rt.position = vec3(-1., 0., 0.);

	scene.root_entity.add_child(&plane);
	plane.add_child(&metis);

	plane.active = true;


	// RENDER LOOP
	app.rendering_loop(GlfwEventMethod::POLL);

	plane_mesh.destroy();
	tex_1.destroy();
	tex_2.destroy();
	basic_shader.destroy();

    return 0;
}

void set_keybindings(Application &app)
{
    //
    // Key mappings :)
    app.key_pressed_map[GLFW_KEY_W] = [](Application *app) {
        app->active_scene->active_camera.is_moving_forwards = true;
        //
    };
    app.key_pressed_map[GLFW_KEY_S] = [](Application *app) {
        app->active_scene->active_camera.is_moving_backwards = true;
    };
    app.key_pressed_map[GLFW_KEY_A] = [](Application *app) {
        app->active_scene->active_camera.is_moving_left = true;
    };
    app.key_pressed_map[GLFW_KEY_D] = [](Application *app) {
        app->active_scene->active_camera.is_moving_right = true;
    };

    //
    // KEY RELEASED
    app.key_released_map[GLFW_KEY_W] = [](Application *app) {
        app->active_scene->active_camera.is_moving_forwards = false;
        //
    };
    app.key_released_map[GLFW_KEY_S] = [](Application *app) {
        app->active_scene->active_camera.is_moving_backwards = false;
    };
    app.key_released_map[GLFW_KEY_A] = [](Application *app) {
        app->active_scene->active_camera.is_moving_left = false;
    };
    app.key_released_map[GLFW_KEY_D] = [](Application *app) {
        app->active_scene->active_camera.is_moving_right = false;
    };

    app.mouse_cursor_action = [](Application *app, vec2 cursor_shift) {
        //         if (current_camera != NULL)
        // 		{
        // active_scene->active_camera.pitch(cursor_shift.y);
        // 			active_scene->active_camera.yaw(cursor_shift.x);
        // 		}
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		if (trans.change_alpha)
			trans.alpha += cursor_shift.x / 200.;
		if (trans.change_beta)
			trans.beta += cursor_shift.y / 300.;
		if (trans.change_gamma)
			trans.gamma += cursor_shift.x / 300.;
    };

    app.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](Application *app) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.change_alpha = true;
	};

    app.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](Application *app) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.change_beta = true;
	};

    app.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](Application *app) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.change_gamma = true;
	};

    app.mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](Application *app) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.change_alpha = false;
	};

    app.mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](Application *app) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.change_beta = false;
	};

    app.mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](Application *app) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.change_gamma = false;
	};

    app.mouse_scrolled_action = [](Application *app, vec2 offset) {
		Entity *ent = app->active_scene->get_active_entity();
		if (ent == NULL)
			return;
		Transform &trans = ent->get_component<Transform>();
		trans.scale += offset.y / 50.f * vec3(1.0);
	};
}
