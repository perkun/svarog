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
	int lc_num_points = 360, ao_size = 400, radar_size = 200;
	string radar_image_filename, radar_fits_filename,
		   ao_image_filename, ao_fits_filename,
		   mag_filename, flux_filename;
};



class ObsStoragePack
{
public:
    ObsStoragePack();
    ~ObsStoragePack();

    static vector<ObsPoint> deserialize_storage(string filename);

    void save_current(const string filepath, bool export_obs = false);
    void save(int id, const string filepath, bool export_obs = false);
    // 	int load(string filename);

    void detach_current_ghosts();
	void detach_all_ghosts();
    void delete_current_observations();

    int add_new_storage(string name);

    string get_current_name();
    string get_name(int id);

    void delete_current();

    template <typename T> T* add_series(string name)
    {
        // TODO check if name exists
        obs_pack[current_id].series_map[name] = new T;
		return static_cast<T *>(obs_pack[current_id].series_map[name]);
    }

    template <typename T> T *get_series(string name)
    {
        if (obs_pack[current_id].series_map.find(name) !=
            obs_pack[current_id].series_map.end())
            return static_cast<T *>(obs_pack[current_id].series_map[name]);

		return NULL;
    }

    int size()
    {
        return obs_pack.size();
    }
    int current_id = 0;

	void current_obs_lambda(const function<void(Observation *)> &func);

private:
    struct ObsStorage
    {
        string name = "untitled";
        map<string, ObservationSeries *> series_map;
    };

    vector<ObsStorage> obs_pack;
    string fix_storage_name(string name, bool exclude_current = false);
    YAML::Node serialize(int id, bool export_obs, string filepath = "");
};

#endif /* OBSERVATIONSTORE_H_ */
