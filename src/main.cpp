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
	Mesh *square_mesh = svarog.create_mesh("/home/perkun/models/erosNEAR.obj", IndexModel::PER_VERTEX);
// 	Mesh *square_mesh = svarog.create_mesh("../data/metis.obj", IndexModel::PER_FACE);
	Transform trans;

	SceneNode *square = new SceneNode(trans, square_mesh);
	square->bind_shader(&basic_shader);
	svarog.scene_graph.root->add_child(square);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// RENDER STUFF
	glClear(GL_COLOR_BUFFER_BIT);
	svarog.scene_graph.draw_depth_first(svarog.scene_graph.root);
	glfwSwapBuffers(svarog.window);

	getchar();

	return 0;
}
