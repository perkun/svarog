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


struct ObsPoint
{
	double jd;
	vec3 observer_pos, target_pos;
	char obs_type = 0b0;
	int lc_num_points = 360, ao_size = 400;
	string radar_image_filename, radar_fits_filename,
		   ao_image_filename, ao_fits_filename,
		   mag_filename, flux_filename;
};



class ObsStoragePack
{
public:
	ObsStoragePack();
	~ObsStoragePack();

	static vector<ObsPoint> import_obs_points(string filename);

	void save_current(const string filepath, bool export_obs = false);
	void save(int id, const string filepath, bool export_obs = false);
// 	int load(string filename);

	void detach_current_ghosts();
	void delete_current_observations();

	int add_new(string name);

	string get_current_name();
	string get_name(int id);

	void delete_current();

	LightcurveSeries* get_current_lightcurves();
	LightcurveSeries* get_current_obs_lightcurves();
	ImageSeries* get_current_ao_images();
	ImageSeries* get_current_radar_images();

	LightcurveSeries* get_lightcurves(int id);
	LightcurveSeries* get_obs_lightcurves(int id);
	ImageSeries* get_ao_images(int id);
	ImageSeries* get_radar_images(int id);

	int size() { return obs_storages.size(); }
	int current_id = 0;

private:
	struct ObsStorage
	{
		string name = "untitled";

		LightcurveSeries* obs_lightcurves;
		LightcurveSeries* lightcurves;
		ImageSeries* ao_images;
		ImageSeries* radar_images;
	};

	vector<ObsStorage> obs_storages;
	string fix_storage_name(string name, bool exclude_current = false);
	YAML::Node serialize(int id, bool export_obs, string filepath = "");
};

#endif /* OBSERVATIONSTORE_H_ */
