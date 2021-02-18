#ifndef OBSERVATORYPANEL_H_
#define OBSERVATORYPANEL_H_

#include "Entity.h"
#include "Lightcurve.h"
#include "Observation.h"
#include "ObservationSeries.h"
#include "ObservationStorage.h"
#include "AoImage.h"
#include "RadarImage.h"
#include "Texture.h"
#include "Utils/File.h"

using namespace std;

class MainLayer;

class ObservatoryPanel
{
public:
    ObservatoryPanel() = default;
	ObservatoryPanel(MainLayer*, double*);
	~ObservatoryPanel();

	void on_imgui_render();

	double *julian_day;

private:
	char buff[100];
	MainLayer *layer;

	void make_lightcurve(Entity &target, Entity &observer, LightcurveSeries*);
	void display_lightcurves(LightcurveSeries*);
	void display_images(ImageSeries*);
	void make_ao_image(Entity &target, Entity &observer, ImageSeries*);
	void make_radar_image(Entity &target, Entity &observer, ImageSeries*);
	void set_target_and_observer(Observation*);
	void observations_panel();

	void set_current_ghosts(ObservationStorage*);

	void menu_bar();
	void observe_points(ObservationStorage*);

	void load_obs_storage(string filepath);

	int ao_size = 400;
	vec4 ao_bg_color = vec4(0.0, 0.0, 0.0, 1.);

	int lc_num_points = 360;

	float angular_speed = 10; // rot speed, sort of
	int radar_size = 200;

// 	int current_storage = 0;
	ObservationStorage *obs_storage;


	bool earth_tilt = true;
};



#endif /* OBSERVATORYPANEL_H_ */
