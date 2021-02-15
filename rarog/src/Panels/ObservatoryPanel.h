#ifndef OBSERVATORYPANEL_H_
#define OBSERVATORYPANEL_H_

#include "MainLayer.h"
#include "Entity.h"
#include "Lightcurve.h"
#include "Observation.h"
#include "ObservationSeries.h"
#include "ObservationStorage.h"
// #include "AoImage.h"
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

	void make_lightcurve(Entity &target, Entity &observer, LightcurveSeries&);
	void display_lightcurves(LightcurveSeries&);

	void display_images(ImageSeries&);

	void make_ao_image(Entity &target, Entity &observer, ImageSeries&);
	void make_radar_image(Entity &target, Entity &observer, ImageSeries&);

	void set_target_and_observer(Observation*);

	vector<Entity> get_scene_entities();
	vector<Entity> get_scene_root_children();
	void append_children(vector<Entity> &ents, Entity entity);

	// sub-panels functions
	void target_selection_panel(vector<Entity>&);
	void observer_selection_panel(vector<Entity>&);
	void observations_panel();

	int ao_size = 400;
	vec4 ao_bg_color = vec4(0.0, 0.0, 0.0, 1.);

	int lc_num_points = 360;

	float angular_speed = 10; // rot speed, sort of
	int radar_size = 200;

	int current_storage = 0;
	vector<ObservationStorage> obs_storage;

	int selected_target_idx = 0;
	int selected_observer_idx = 0;

	bool earth_tilt = true;
};



#endif /* OBSERVATORYPANEL_H_ */
