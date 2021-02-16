#ifndef OBSERVATIONSTORAGE_H_
#define OBSERVATIONSTORAGE_H_

#include "ObservationSeries.h"
#include "Utils/File.h"

using namespace std;

struct YamlPoint
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

	void delete_all_observations();

	string filename = "untitled.yaml";
	string name = "untitled";

	LightcurveSeries *lightcurves;
	ImageSeries *ao_images;
	ImageSeries *radar_images;

	vector<YamlPoint> points;
	bool file_loaded = false;

};

#endif /* OBSERVATIONSTORE_H_ */
