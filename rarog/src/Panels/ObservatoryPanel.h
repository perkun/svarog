#ifndef OBSERVATORYPANEL_H_
#define OBSERVATORYPANEL_H_

#include "MainLayer.h"
#include "Entity.h"
#include "Lightcurve.h"
#include "AoImage.h"
#include "RadarImage.h"
#include "Texture.h"

using namespace std;


class ObservatoryPanel
{
public:
    ObservatoryPanel() = default;
	ObservatoryPanel(MainLayer*);
	~ObservatoryPanel();

	void on_imgui_render();

private:
	MainLayer *layer;

	void observe_button();

	void make_lightcurve(Entity &target, Entity &observer);
	void display_lightcurves();

	void make_ao_image(Entity &target, Entity &observer);
	void display_ao_images();

	void make_radar_image(Entity &target, Entity &observer);
	RadarImage construct_delay_doppler(float*, float*, float*, int, int);
	void display_radar_images();


	void set_target_and_observer(Entity&, Entity&, Entity&, Entity&);

	vector<Entity> get_scene_entities();
	vector<Entity> get_scene_root_children();
	void append_children(vector<Entity> &ents, Entity entity);

	// sub-panels functions
	void target_selection_panel(vector<Entity>&);
	void observer_selection_panel(vector<Entity>&);
	void observations_panel();

	int lc_id = 0;
	int ao_id = 0;
	int radar_id = 0;

	float radar_factor = 10;

	int ao_size = 400;

	int lc_num_points = 360;
	float lcs_min = 1e38;
	float lcs_max = -1e38;

	int selected_target_idx = 0;
	int selected_observer_idx = 0;

	vec4 ao_bg_color = vec4(0.0, 0.0, 0.0, 1.);

	bool earth_tilt = true;

	vector<Lightcurve> lightcurves;
	vector<AoImage> ao_images;
	vector<RadarImage> radar_images;
};

#endif /* OBSERVATORYPANEL_H_ */
