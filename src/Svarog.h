#ifndef SVAROG_H_
#define SVAROG_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
// #include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "MemmoryLedger.h"
#include "Mesh.h"
#include "Transform.h"
#include "SceneGraph.h"


using namespace std;

class Svarog
{
public:
	Svarog(int, int, string, bool visible = true);
	~Svarog();
	void show_window();
	void hide_window();
	Mesh* create_mesh();

	SceneGraph scene_graph;
	GLFWwindow* window;

private:
	void create_window(int, int, string title, bool visible);
	void destroy_window();

	MemmoryLedger<Mesh*> mesh_ledger;

};

#endif /* SVAROG_H_ */
