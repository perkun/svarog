#include "svpch.h"
#include "ObservationStorage.h"


ObservationStorage::ObservationStorage()
{
	lightcurves = new LightcurveSeries;
	ao_images = new ImageSeries;
	radar_images = new ImageSeries;
}


ObservationStorage::~ObservationStorage()
{
	delete lightcurves;
	delete ao_images;
	delete radar_images;
}


void ObservationStorage::save(const string filepath)
{
	this->filename = filepath;
	this->name = File::remove_extension(File::file_base(filename));

	YAML::Emitter out;
    out << YAML::BeginMap;
	out << YAML::Key << "points" << YAML::BeginSeq;

	// serialize Observation

    out << YAML::EndSeq;
    out << YAML::EndMap;


    std::ofstream fout(filepath);
    fout << out.c_str();
}


bool ObservationStorage::load(const string filename)
{
	file_loaded = false;
	this->filename = filename;
	this->name = File::remove_extension(File::file_base(filename));


    YAML::Node data = YAML::LoadFile(filename);
    if (!data["points"])
        return false;


	for (auto yaml_point: data["points"])
	{
		YamlPoint p;
		p.jd = yaml_point["jd"].as<double>();

		p.observer.x = yaml_point["observer_position"][0].as<float>();
		p.observer.y = yaml_point["observer_position"][1].as<float>();
		p.observer.z = yaml_point["observer_position"][2].as<float>();

		p.target.x = yaml_point["target_position"][0].as<float>();
		p.target.y = yaml_point["target_position"][1].as<float>();
		p.target.z = yaml_point["target_position"][2].as<float>();

		points.push_back(p);
	}

	file_loaded = true;
	return true;
}


void ObservationStorage::detach_all_ghosts()
{
	lightcurves->detach_all_ghosts();
	ao_images->detach_all_ghosts();
	radar_images->detach_all_ghosts();
}


void ObservationStorage::delete_all_observations()
{
    while (lightcurves->get_current_obs() != NULL)
        lightcurves->delete_current_obs();

    while (ao_images->get_current_obs() != NULL)
        ao_images->delete_current_obs();

    while (radar_images->get_current_obs() != NULL)
        radar_images->delete_current_obs();
}
