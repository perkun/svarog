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

using namespace std;

void print_id(SceneNode *n)
{
    cout << n << endl;
}




////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main()
{
    cout << "This is Svarog!" << endl;

    Svarog svarog(1200, 1200, "RenderEngine window");

    // Key mappings :)
    svarog.key_pressed_map[GLFW_KEY_W] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_forwards = true;

    };
    svarog.key_pressed_map[GLFW_KEY_S] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_backwards = true;
    };
    svarog.key_pressed_map[GLFW_KEY_A] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_left = true;
    };
    svarog.key_pressed_map[GLFW_KEY_D] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_right = true;
    };


// KEY RELEASEK
    svarog.key_released_map[GLFW_KEY_W] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_forwards = false;

    };
    svarog.key_released_map[GLFW_KEY_S] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_backwards = false;
    };
    svarog.key_released_map[GLFW_KEY_A] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_left = false;
    };
    svarog.key_released_map[GLFW_KEY_D] = [](SceneNode *current_node,
                                            Camera *current_camera) {
        if (current_camera != NULL)
			current_camera->is_moving_right = false;
    };


    Shader basic_shader;
    basic_shader.set_uniform_4fv("u_color", vec4(1., 1., 1., 1.));

    // 	Mesh *square_mesh = svarog.create_mesh("../data/square.obj");
    // 	Mesh *square_mesh = svarog.create_mesh("../data/ico.obj",
    // IndexModel::PER_FACE); 	Mesh *square_mesh =
    // svarog.create_mesh("/home/perkun/models/sphere.obj");
    Mesh *eros_mesh = svarog.create_mesh("/home/perkun/models/erosNEAR.obj",
                                         IndexModel::PER_FACE);
    Mesh *metis_mesh = svarog.create_mesh("/home/perkun/models/metisSAGE.obj",
                                          IndexModel::PER_FACE);

    SceneNode *eros = new SceneNode(eros_mesh);
    eros->bind_shader(&basic_shader);
    svarog.scene_graph_root->add_child(eros);


    eros->transform.position = vec3(0, 0, 0);
    // 	eros->transform.scale = vec3(0.8, 0.8, 0.8);
    eros->transform.scale = vec3(1.0);
    eros->transform.alpha = 1.0;
    // 	eros->transform.beta = 1.0;

    SceneNode *metis = new SceneNode(metis_mesh);
    metis->bind_shader(&basic_shader);
    eros->add_child(metis);

    metis->transform.position = vec3(2, 0., 0.0);
    metis->transform.scale = vec3(0.2, 0.2, 0.2);
    metis->transform.beta = 0;

    svarog.current_node = metis;

    Camera *camera = new Camera(vec3(0, -10, 0), eros->transform.position,
                                radians(45.0), 1.0, 0.1, 100.0);
	camera->speed = 1.;
    svarog.current_camera = camera;

    double time = glfwGetTime();
    double previous_time;
    double time_delta;
    while (!glfwWindowShouldClose(svarog.window->winptr))
    {
		previous_time = time;
		time = glfwGetTime();
		time_delta = time - previous_time;


        // RENDER STUFF
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        svarog.scene_graph_root->update_depth_first();
        svarog.scene_graph_root->draw_depth_first(camera->get_view(),
                                                  camera->get_perspective());
        glfwSwapBuffers(svarog.window->winptr);
        glfwPollEvents();

		svarog.current_camera->move(time_delta);
    }

	delete camera;
    return 0;
}
