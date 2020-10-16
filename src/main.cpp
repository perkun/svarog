#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Svarog.h"
#include "Shader.h"


using namespace std;


void print_id(SceneNode* n)
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

	Shader basic_shader;
	basic_shader.set_uniform_4fv("u_color", vec4(1., 1., 1., 1.));

// 	Mesh *square_mesh = svarog.create_mesh("../data/square.obj");
// 	Mesh *square_mesh = svarog.create_mesh("../data/ico.obj", IndexModel::PER_FACE);
// 	Mesh *square_mesh = svarog.create_mesh("/home/perkun/models/sphere.obj");
	Mesh *eros_mesh = svarog.create_mesh("/home/perkun/models/erosNEAR.obj", IndexModel::PER_VERTEX);
	Mesh *metis_mesh = svarog.create_mesh("../data/metis.obj", IndexModel::PER_FACE);


	SceneNode *eros = new SceneNode(eros_mesh);
	eros->bind_shader(&basic_shader);
	svarog.scene_graph_root->add_child(eros);

	eros->transform.position = vec3(0.5, 0.5, 0.0);
	eros->transform.scale = vec3(0.2, 0.2, 0.2);
	eros->transform.alpha = 1.0;
// 	eros->transform.beta = 1.0;


	SceneNode *metis = new SceneNode(metis_mesh);
	metis->bind_shader(&basic_shader);
	eros->add_child(metis);

// 	metis->transform.position = vec3(-10.0, -1., 0.0);
	metis->transform.scale = vec3(0.2, 0.2, 0.2);
	metis->transform.beta = 0;




	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// RENDER STUFF
	glClear(GL_COLOR_BUFFER_BIT);

	svarog.scene_graph_root->update_depth_first();
	svarog.scene_graph_root->draw_depth_first();

	glfwSwapBuffers(svarog.window);

	getchar();

	return 0;
}
