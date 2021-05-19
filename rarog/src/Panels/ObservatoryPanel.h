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

class ImGuiCanvas
{
public:
    ImGuiCanvas() {}
    ~ImGuiCanvas() {}

    void prepare();
    void add_line(double x1, double y1, double x2, double y2,
					double x_min, double x_max,
					double y_min, double y_max, ImU32 color);
	void draw();
private:
	ImDrawList *draw_list;
	ImVec2 origin;
	ImVec2 canvas_sz;
};

class ObservatoryPanel
{
public:
    ObservatoryPanel() = default;
	ObservatoryPanel(MainLayer*, ObsStoragePack*);
	~ObservatoryPanel();

	void on_imgui_render();

private:
	char buff[100];
	MainLayer *layer;

	void lightcurves_tab();
	void display_images(ImageSeries*);
	void observations_panel();

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
