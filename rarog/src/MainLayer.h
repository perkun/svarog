#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <cmdlineargs/ArgumentHandler.h>

#include "Layer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"
#include "Batch.h"
#include "Panels/SceneHierarchyPanel.h"
// #include "Svarog.h"


using namespace std;
using namespace glm;


enum class Mode {
	EDITOR = 0,
	RUNTIME,
	NUM_MODES
};


class MainLayer : public Layer
{
public:
    MainLayer(int argc, char *argv[]);
	~MainLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(double ts) override;
	virtual void on_imgui_render() override;

	virtual void on_event(Event&) override;
	void on_key_released_event(KeyReleasedEvent&);
	void on_window_resize_event(WindowResizeEvent&);

	// GUI functions
	void scene_window();
	void scene_options_panel();
	void menu_bar();

	void toggle_mode();


	IndexedModel create_grid(float size, float sep, float alpha);

	bool show_imgui_demo = false;
	bool show_scene_options = true;
	bool show_grid = true;

private:
	vec2 viewport_panel_size;
	ArgumentHandler arg_handler;

	EditorCamera editor_camera;

// 	Shader *basic_shader, *color_shader, *line_shader;;

	Scene scene;

// 	VertexArrayObject *model_vao;
	Texture *texture = NULL;

	Entity grid;

	Mode mode = Mode::EDITOR;

	int guizmo_type = -1;

	SceneHierarchyPanel scene_hierarchy_panel;

// 	double fps;
// 	double fps_history[100];
};

#endif /* MAINLAYER_H_ */
