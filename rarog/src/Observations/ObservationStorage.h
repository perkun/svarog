#ifndef OBSERVATIONSTORAGE_H_
#define OBSERVATIONSTORAGE_H_

#include "ObservationSeries.h"
#include "Utils/File.h"

using namespace std;

enum ObsType {
	LC = 0b1,
	AO = 0b10,
	RADAR = 0b100
};

struct YamlPoint
{
	double jd;
	vec3 observer, target;
	char obs_types = 0b0;
	int lc_num_points = 360, ao_size = 400;
};

class ObservationStorage
{
public:
	ObservationStorage();
	~ObservationStorage();

	void save(const string filepath);
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
