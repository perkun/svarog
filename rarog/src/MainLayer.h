#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <cppargs/Parser.h>
#include "Layer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"
#include "Batch.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Framebuffer.h"
#include "ObservatoryPanel.h"
#include "ObservePanel.h"
#include "TimePanel.h"
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
	friend class ObservatoryPanel;
	friend class ObservePanel;
public:
    MainLayer(Args args);
	~MainLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(double ts) override;
	virtual void on_imgui_render() override;

	virtual void on_event(Event&) override;
	void on_key_released_event(KeyReleasedEvent&);
	void on_window_resize_event(WindowResizeEvent&);

private:
	void create_default_scene();
	IndexedModel create_grid(float size, float sep, float alpha);
	// GUI functions
	void scene_window();
	void scene_options_panel();
	void menu_bar();
	void toggle_mode();
	void set_editor_mode();
	void set_runtime_mode();


	// observations
	void make_lightcurve(LightcurveSeries*, int num_points);
	void make_ao_image(ImageSeries*, int, bool, vec4);
	void make_radar_image(ImageSeries*, int, float);

	void load_obs_storage(string filepath);
	void load_damit_lc(string filepath);
	void observe_obs_points(const vector<ObsPoint> &obs_points);

	void set_target_and_observer(Observation*);
	void set_ghosts(Observation*);
	void set_current_ghosts();

	bool shadow_map = true;
	bool show_imgui_demo = false;
	bool show_scene_options = true;
	bool show_grid = true;

	vec2 viewport_panel_size;

	Args args;

	EditorCamera editor_camera;
	Scene scene, ui_scene;

	Entity grid;
	Mode mode = Mode::EDITOR;
	int guizmo_type = -1;
	Framebuffer *ms_framebuffer, *framebuffer;

	bool multisampling = false;

	SceneHierarchyPanel scene_hierarchy_panel;
	ObservatoryPanel observatory_panel;
	ObservePanel observe_panel;
	TimePanel time_panel;

	ObsStoragePack obs_pack;


// 	double fps;
// 	double fps_history[100];
};

#endif /* MAINLAYER_H_ */
