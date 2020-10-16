#include "Svarog.h"

Svarog::Svarog(int width, int height, string w_title, bool visible)
{
	if (!glfwInit())
	{
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		exit(0);
	}

	create_window(width, height, w_title, visible);
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	GLenum err = glewInit();
	if (err != GLEW_OK)
		cout << "glew init failed" << endl;


// 	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace (GL_CCW);

	scene_graph_root = new SceneNode();
}


Svarog::~Svarog()
{

	mesh_ledger.delete_all();

	delete scene_graph_root;
	glfwTerminate();
}

Mesh* Svarog::create_mesh(string filename, int mode)
{
	Mesh *m = new Mesh(filename, mode);
	mesh_ledger.add(m);
	return m;
}

void Svarog::create_window(int w, int h, string title, bool visible)
{
	if (visible)
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	else
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	// new window focus
// 	glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);

	window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
}

void Svarog::destroy_window()
{
	glfwDestroyWindow(window);
}

void Svarog::show_window()
{
	glfwShowWindow(window);
}

void Svarog::hide_window()
{
	glfwHideWindow(window);
}

