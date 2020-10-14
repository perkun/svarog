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
}


Svarog::~Svarog()
{

	mesh_ledger.delete_all();

	delete scene_graph.root;
	glfwTerminate();
}

Mesh* Svarog::create_mesh()
{
	Mesh *m = new Mesh();
	mesh_ledger.add(m);
	return m;
}

void Svarog::create_window(int, int, string title, bool visible)
{
	if (visible)
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	else
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	// new window focus
// 	glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);

	window = glfwCreateWindow(640, 480, title.c_str(), NULL, NULL);
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

