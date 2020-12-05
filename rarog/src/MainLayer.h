#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmdlineargs/ArgumentHandler.h>

#include "Svarog.h"

#include "CameraController.h"
#include "ModelController.h"
// #include "CameraControllerFPP.h"



using namespace std;

class MainLayer : public SceneLayer
{
public:
    MainLayer(int argc, char *argv[]);
	~MainLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	virtual void on_imgui_render() override;

	// GUI functions
	void scene_window();
	void scene_options();
	void orbital_parameters_panel();
	void menu_bar();

	void load_model();
	void load_texture();
	void remove_texture();

	bool show_imgui_demo = false;
	bool show_scene_options = true;
	bool show_grid = true;

private:
	vec2 viewport_panel_size;
	ArgumentHandler arg_handler;

	Shader *basic_shader, *color_shader, *line_shader;;

	VertexArrayObject *model_vao;
	Texture *texture = NULL;

	Entity model, grid;
};

#endif /* MAINLAYER_H_ */
