#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <cmdlineargs/ArgumentHandler.h>

#include "Layer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"
#include "Batch.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Framebuffer.h"
#include "Lightcurve.h"
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

private:
	IndexedModel create_grid(float size, float sep, float alpha);
	// GUI functions
	void scene_window();
	void scene_options_panel();
	void menu_bar();
	void toggle_mode();
	void set_editor_mode();
	void set_runtime_mode();

	void make_lightcurve(Entity &target);

	Entity light, runtime_observer, model;

	bool shadow_map = true;
	bool show_imgui_demo = false;
	bool show_scene_options = true;
	bool show_grid = true;

	vec2 viewport_panel_size;
	ArgumentHandler arg_handler;

	EditorCamera editor_camera;
	Scene scene, ui_scene;

	Entity grid;
	Mode mode = Mode::EDITOR;
	int guizmo_type = -1;
	SceneHierarchyPanel scene_hierarchy_panel;
	Framebuffer *ms_framebuffer, *framebuffer;

	vector<Lightcurve> lightcurves;
	int lc_id;

// 	double fps;
// 	double fps_history[100];
};

#endif /* MAINLAYER_H_ */
