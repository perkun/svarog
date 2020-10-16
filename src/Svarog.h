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
#include "SceneNode.h"


using namespace std;

class Svarog
{
public:
	Svarog(int, int, string, bool visible = true);
	~Svarog();
	void show_window();
	void hide_window();
	Mesh* create_mesh(string filename, int mode);

// 	SceneGraph scene_graph;
	SceneNode *scene_graph_root;
	GLFWwindow* window;

private:
	void create_window(int, int, string title, bool visible);
	void destroy_window();

	MemmoryLedger<Mesh*> mesh_ledger;

};

#endif /* SVAROG_H_ */
