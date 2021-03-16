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

struct ObsPack
{
	vector<YamlPoint> points;

	string filename = "untitled.yaml";
	string name = "untitled";
	bool file_loaded = false;

	LightcurveSeries* lightcurves;
	ImageSeries* ao_images;
	ImageSeries* radar_images;
};


class ObservationStorage
{
public:
	ObservationStorage();
	~ObservationStorage();

	YAML::Node serialize();
	void save(const string filepath);
	bool load(string filename);

	void detach_current_ghosts();
	void delete_current_observations();

	void add_new(string name);

	vector<YamlPoint> get_current_points();
	int get_current_points_size();
	string get_current_name();
	string get_name(int id);

	void delete_current();

	LightcurveSeries* get_current_lightcurves();
	ImageSeries* get_current_ao_images();
	ImageSeries* get_current_radar_images();

	LightcurveSeries* get_lightcurves(string name);
	ImageSeries* get_ao_images(string name);
	ImageSeries* get_radar_images(string name);

	int size() { return obs_packs.size(); }
	int current_id = 0;

private:
	vector<ObsPack> obs_packs;
	string fix_storage_name(string name, bool exclude_current = false);
};

#endif /* OBSERVATIONSTORE_H_ */
