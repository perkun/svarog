#ifndef OBSERVATORYPANEL_H_
#define OBSERVATORYPANEL_H_

#include "Entity.h"
#include "Lightcurve.h"
#include "Observation.h"
#include "ObservationSeries.h"
#include "ObservationStorage.h"
#include "Image.h"
#include "Texture.h"
#include "Utils/File.h"

using namespace std;

class MainLayer;

class ObservatoryPanel
{
public:
    ObservatoryPanel() = default;
	ObservatoryPanel(MainLayer*, double*, ObsStoragePack*);
	~ObservatoryPanel();

	void on_imgui_render();

	void load_obs_storage(string filepath);
	void load_lc_file(string filepath);
	void observe_obs_points(const vector<ObsPoint> obs_points);

	double *julian_day;

private:
	char buff[100];
	MainLayer *layer;

	void display_lightcurves(LightcurveSeries*, LightcurveSeries* obs=NULL);
	void display_images(ImageSeries*);

	void make_lightcurve(Entity &target, Entity &observer, LightcurveSeries*, int);
	void make_ao_image(Entity &target, Entity &observer, ImageSeries*, int);
	void make_radar_image(Entity &target, Entity &observer, ImageSeries*, int);

	void set_target_and_observer(Observation*);
	void observations_panel();

	void set_current_ghosts(ObsStoragePack*);

	int ao_size = 400;
	int dd_size = 200;
	vec4 ao_bg_color = vec4(0.0, 0.0, 0.0, 1.);

	int lc_num_points = 360;

	float angular_speed = 10; // rot speed, sort of
	int radar_size = 200;

	ObsStoragePack *obs_storage;

	bool earth_tilt = true;
};



#endif /* OBSERVATORYPANEL_H_ */
