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
    set_keybindings(svarog);

    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  WIN_W / (float)WIN_H, 0.1, 100.0);

    camera.speed = 1.;
    svarog.current_camera = &camera;

    Shader basic_shader("../src/shaders/basic_shader.vs",
						"../src/shaders/basic_shader.fs");

//     basic_shader.set_uniform_4fv("u_color", vec4(1., 1., 1., 1.));
	vec4 color(1.0);
// 	basic_shader.uniforms.push_back(Uniform("u_color", type::VEC4, (void*)&color));
// 	basic_shader.uniforms.push_back(Uniform("view_matrix", type::MAT4, (void*)&camera->view));
// 	basic_shader.uniforms.push_back(Uniform("perspective_matrix", type::MAT4, (void*)&camera->perspective));
// 	basic_shader.uniforms.push_back(Uniform("model_matrix", type::MAT4, (void*)&color));


//     	shader->set_uniform_4fv("u_color", color);
// 		shader->set_uniform_mat4f("view_matrix", view_matrix);
// 		shader->set_uniform_mat4f("perspective_matrix", perspective_matrix);
// 		shader->set_uniform_mat4f("model_matrix", world_transform.model_matrix);

    // 	Mesh *square_mesh = svarog.create_mesh("../data/square.obj");
    // 	Mesh *square_mesh = svarog.create_mesh("../data/ico.obj",
    // IndexModel::PER_FACE); 	Mesh *square_mesh =
    // svarog.create_mesh("/home/perkun/models/sphere.obj");
//     Mesh *eros_mesh = svarog.create_mesh("../data/ico.obj",
    Mesh *eros_mesh = svarog.create_mesh("/home/perkun/models/erosNEAR.obj",
                                         IndexModel::PER_VERTEX);
    Mesh *metis_mesh = svarog.create_mesh("../data/model.obj",
                                          IndexModel::PER_VERTEX);
// 	Mesh *arrow_mesh = svarog.create_mesh("../data/arrow.obj",
// 										  IndexModel::PER_FACE);

	Mesh *plane_mesh = svarog.create_mesh("../data/plane.obj",
										  IndexModel::PER_FACE);

    SceneNode *eros = new SceneNode(eros_mesh);
    eros->bind_shader(&basic_shader);
    eros->transform.position = vec3(0.5, 0., 0.);
    eros->transform.scale = vec3(0.5);
//     eros->transform.alpha = 1.0;
    // 	eros->transform.beta = 1.0;

	SceneNode *ico = new SceneNode(eros_mesh);
	ico->bind_shader(&basic_shader);
	ico->transform.scale = vec3(0.5);
	ico->transform.position = vec3(-0.5, 0., 0.0);


    SceneNode *metis = new SceneNode(metis_mesh);
    metis->bind_shader(&basic_shader);
    metis->transform.position = vec3(0., 0., 0.);
//     metis->transform.scale = vec3(0.2, 0.2, 0.2);
//     metis->transform.beta = 0.;
//
// 	SceneNode *arrow = new SceneNode(arrow_mesh);
// 	arrow->bind_shader(&basic_shader);
// 	arrow->transform.position = vec3(0., 0., 0.);
// 	arrow->transform.scale = vec3(1., 1., 1.5);
// 	arrow->color = vec4(0.7, 0.2, 0.2, 1.0);


	SceneNode *plane = new SceneNode(plane_mesh);
	plane->bind_shader(&basic_shader);
	plane->transform.position = vec3(0., 2., 0.);
	plane->transform.beta = 1;
	plane->transform.gamma = 1;
	plane->transform.scale = vec3(2.f);

// 	Texture fractal("../data/fraktal.png");
	Texture bike("../data/makeeva2.png");
	Texture photo("../data/forest.jpg");
//
// 	ico->texture = &fractal;
// 	eros->texture = &bike;
	plane->texture = &bike;

	metis->bind_texture(&photo);

	// ad children
	svarog.scene_graph_root->add_child(metis);
	svarog.scene_graph_root->add_child(plane);
//
//     svarog.scene_graph_root->add_child(eros);
//     svarog.scene_graph_root->add_child(ico);
// 	eros->add_child(arrow);
//     eros->add_child(metis);

	svarog.scene_graph_root->camera = &camera;

    svarog.current_node = plane;


    double time = glfwGetTime();
    double previous_time;
    double time_delta;
    while (!glfwWindowShouldClose(svarog.window->winptr))
    {

		previous_time = time;
		time = glfwGetTime();
		time_delta = time - previous_time;

        // RENDER STUFF
		glClearColor(BG_COLOR);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        svarog.scene_graph_root->update_depth_first();
        svarog.scene_graph_root->draw_depth_first();

        glfwSwapBuffers(svarog.window->winptr);
//         glfwPollEvents();
		glfwWaitEvents();

		svarog.current_camera->move(time_delta);
    }

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
		svarog->current_node->transform.change_alpha = true;
	};

	svarog.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](Svarog *svarog) {
		svarog->current_node->transform.change_beta = true;
	};

	svarog.mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](Svarog *svarog) {
		svarog->current_node->transform.change_gamma = true;
	};


	svarog.mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](Svarog *svarog) {
		svarog->current_node->transform.change_alpha = false;
	};

	svarog.mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](Svarog *svarog) {
		svarog->current_node->transform.change_beta = false;
	};

	svarog.mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](Svarog *svarog) {
		svarog->current_node->transform.change_gamma = false;
	};

	svarog.mouse_scrolled_action = [](Svarog *svarog, vec2 offset) {
		svarog->current_node->transform.scale += offset.y / 50.f * vec3(1.0);
	};


}
