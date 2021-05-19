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

    template <typename T> T* add_series(string name)
    {
        // TODO check if name exists
        obs_storages[current_id].storage[name] = new T;
		return static_cast<T *>(obs_storages[current_id].storage[name]);
    }

    template <typename T> T *get_series(string name)
    {
        if (obs_storages[current_id].storage.find(name) !=
            obs_storages[current_id].storage.end())
            return static_cast<T *>(obs_storages[current_id].storage[name]);

		return NULL;
    }

    int size()
    {
        return obs_storages.size();
    }
    int current_id = 0;

	void current_obs_lambda(const function<void(Observation *)> &func);

private:
    struct ObsStorage
    {
        string name = "untitled";
        map<string, ObservationSeries *> storage;
    };

    vector<ObsStorage> obs_storages;
    string fix_storage_name(string name, bool exclude_current = false);
    YAML::Node serialize(int id, bool export_obs, string filepath = "");
};

#endif /* OBSERVATIONSTORE_H_ */
