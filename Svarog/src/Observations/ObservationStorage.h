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

	string filename = "untitled.storage";
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

	void save_current(const string filepath, bool export_obs = false);
	void save(int id, const string filepath, bool export_obs = false);
	int load(string filename);

	void detach_current_ghosts();
	void delete_current_observations();

	int add_new(string name);

	vector<YamlPoint> get_current_points();
	int get_current_points_size();
	string get_current_name();
	string get_name(int id);

	void delete_current();

	LightcurveSeries* get_current_lightcurves();
	ImageSeries* get_current_ao_images();
	ImageSeries* get_current_radar_images();

	LightcurveSeries* get_lightcurves(int id);
	ImageSeries* get_ao_images(int id);
	ImageSeries* get_radar_images(int id);

	int size() { return obs_packs.size(); }
	int current_id = 0;

private:
	vector<ObsPack> obs_packs;
	string fix_storage_name(string name, bool exclude_current = false);
	YAML::Node serialize(int id, bool export_obs, string filepath = "");
};

#endif /* OBSERVATIONSTORE_H_ */
