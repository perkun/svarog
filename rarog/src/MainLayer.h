#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <cmdlineargs/ArgumentHandler.h>

#include "SceneLayer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"
#include "Batch.h"
// #include "Svarog.h"


using namespace std;
using namespace glm;

enum class Mode {
	EDITOR = 0,
	RUNTIME,
	NUM_MODES
};

class MainLayer : public SceneLayer
{
public:
    MainLayer(int argc, char *argv[]);
	~MainLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(double ts) override;
	virtual void on_event(Event&) override;
	virtual void on_imgui_render() override;

	// GUI functions
	void scene_window();
	void scene_options();
	void orbital_parameters_panel();
	void menu_bar();

	void toggle_mode();

	void load_model();
	void load_texture();
	void remove_texture();

	IndexedModel create_grid(float size, float sep, float alpha);

	bool show_imgui_demo = false;
	bool show_scene_options = true;
	bool show_grid = true;

private:
	vec2 viewport_panel_size;
	ArgumentHandler arg_handler;

	EditorCamera editor_camera;

	Shader *basic_shader, *color_shader, *line_shader;;

	VertexArrayObject *model_vao;
	Texture *texture = NULL;

	Entity model, grid;

	Mode mode = Mode::EDITOR;
};

#endif /* MAINLAYER_H_ */
