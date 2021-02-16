#ifndef OBSERVATIONSTORAGE_H_
#define OBSERVATIONSTORAGE_H_

#include "ObservationSeries.h"

using namespace std;

struct Point
{
	double jd;
	vec3 observer, target;
};

class ObservationStorage
{
public:
	ObservationStorage();
	~ObservationStorage();

	void save();
	bool load(string filename);
	void detach_all_ghosts();

	string filename = "untitled.yaml";

	LightcurveSeries *lightcurves;
	ImageSeries *ao_images;
	ImageSeries *radar_images;

	vector<Point> points;

};

#endif /* OBSERVATIONSTORE_H_ */
