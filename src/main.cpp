#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Svarog.h"
#include "Texture.h"
#include "Transform.h"
// #include "vendor/entt/entt.hpp"
#include "Entity.h"

#define WIN_W 800
#define WIN_H 600
#define BG_COLOR 41/256., 46/256., 48/256., 1.0

using namespace std;

void print_id(SceneNode *n)
{
    cout << n << endl;
}


void set_keybindings(Svarog &svarog);



////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main()
{
    cout << "This is Svarog!" << endl;

    Svarog svarog(WIN_W, WIN_H, "Svarog", false);
	Renderer renderer;
    set_keybindings(svarog);

    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  WIN_W / (float)WIN_H, 0.1, 100.0);

    camera.speed = 1.;
    svarog.current_camera = &camera;

    Shader basic_shader("../src/shaders/basic_shader.vs",
						"../src/shaders/basic_shader.fs");


	// Meshes
	Mesh metis_mesh("../data/model.obj", IndexModel::PER_VERTEX);
	Mesh plane_mesh("../data/plane.obj", IndexModel::PER_VERTEX);

	// Textures
	Texture tex_1("../data/cyber.png");
	Texture tex_2("../data/horsehead.jpeg");

	// Create scene
	Scene scene;


	// Entities

	Entity plane = scene.create_entity();
	plane.add_component<Mesh>(plane_mesh);
	plane.add_component<Texture>(tex_1);
	plane.add_component<Shader>(basic_shader);

	Transform &tr = plane.get_component<Transform>();
	tr.beta = 1;
	tr.position = vec3(-1.0, 0., 0.);

	Entity metis = scene.create_entity("Metis ent");
	metis.add_component<Mesh>(metis_mesh);
	metis.add_component<Texture>(tex_2);
	metis.add_component<Shader>(basic_shader);
	Transform &metis_trans = metis.get_component<Transform>();
	metis_trans.position = vec3(1.5, 0.0, 0.0);


	Entity root_entity = scene.create_entity("Root");
	Transform &rt = root_entity.get_component<Transform>();
	rt.position = vec3(0., 0., 0.5);

	root_entity.add_child(&plane);
	plane.add_child(&metis);

	// RENDER LOOP
    double time = glfwGetTime();
    double previous_time;
    double time_delta;
    while (!glfwWindowShouldClose(svarog.window->winptr))
    {

		previous_time = time;
		time = glfwGetTime();
		time_delta = time - previous_time;

        // RENDER STUFF
		renderer.clear(BG_COLOR);

		scene.draw_depth_first(&root_entity, &camera);

        glfwSwapBuffers(svarog.window->winptr);
//         glfwPollEvents();
		glfwWaitEvents();

		svarog.current_camera->move(time_delta);
    }

	plane_mesh.destroy();
	tex_1.destroy();
	tex_2.destroy();
	basic_shader.destroy();

    return 0;
}

void set_keybindings(Svarog &svarog)
{

    // Key mappings :)
   svarog.key_pressed_map[GLFW_KEY_W] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_forwards = true;

    };
    svarog.key_pressed_map[GLFW_KEY_S] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_backwards = true;
    };
    svarog.key_pressed_map[GLFW_KEY_A] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_left = true;
    };
    svarog.key_pressed_map[GLFW_KEY_D] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_right = true;
    };


	// KEY RELEASED
    svarog.key_released_map[GLFW_KEY_W] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_forwards = false;

    };
    svarog.key_released_map[GLFW_KEY_S] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_backwards = false;
    };
    svarog.key_released_map[GLFW_KEY_A] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_left = false;
    };
    svarog.key_released_map[GLFW_KEY_D] = [](Svarog *svarog) {
        if (svarog->current_camera != NULL)
			svarog->current_camera->is_moving_right = false;
    };

	svarog.mouse_cursor_action = [](Svarog *svarog, vec2 cursor_shift) {
//         if (current_camera != NULL)
// 		{
// 			current_camera->pitch(cursor_shift.y);
// 			current_camera->yaw(cursor_shift.x);
// 		}
		if (svarog->current_node != NULL)
		{
			if (svarog->current_node->transform.change_alpha)
				svarog->current_node->transform.alpha += cursor_shift.x / 200.;
			if (svarog->current_node->transform.change_beta)
				svarog->current_node->transform.beta +=  cursor_shift.y / 300.;
			if (svarog->current_node->transform.change_gamma)
				svarog->current_node->transform.gamma +=  cursor_shift.x / 300.;
		}
	};

	svarog.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](Svarog *svarog) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.change_alpha = true;
	};

	svarog.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](Svarog *svarog) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.change_beta = true;
	};

	svarog.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](Svarog *svarog) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.change_gamma = true;
	};


	svarog.mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](Svarog *svarog) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.change_alpha = false;
	};

	svarog.mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](Svarog *svarog) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.change_beta = false;
	};

	svarog.mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](Svarog *svarog) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.change_gamma = false;
	};

	svarog.mouse_scrolled_action = [](Svarog *svarog, vec2 offset) {
		if (svarog->current_node != NULL)
			svarog->current_node->transform.scale += offset.y / 50.f * vec3(1.0);
	};


}
