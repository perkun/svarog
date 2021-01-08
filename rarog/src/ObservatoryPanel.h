#ifndef OBSERVATORYPANEL_H_
#define OBSERVATORYPANEL_H_

#include "MainLayer.h"
#include "Entity.h"
#include "Lightcurve.h"

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
	void make_lightcurve(Entity &target);
	void display_lightcurves();

	int lc_id = 0;

	vector<Lightcurve> lightcurves;

};

#endif /* OBSERVATORYPANEL_H_ */
