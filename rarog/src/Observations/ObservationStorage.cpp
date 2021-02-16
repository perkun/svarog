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


void ObservationStorage::save()
{

}


bool ObservationStorage::load(const string filename)
{
	this->filename = filename;

    YAML::Node data = YAML::LoadFile(filename);
    if (!data["points"])
        return false;


	for (auto yaml_point: data["points"])
	{
		Point p;
		p.jd = yaml_point["jd"].as<double>();

		p.observer.x = yaml_point["observer_position"][0].as<float>();
		p.observer.y = yaml_point["observer_position"][1].as<float>();
		p.observer.z = yaml_point["observer_position"][2].as<float>();

		p.target.x = yaml_point["target_position"][0].as<float>();
		p.target.y = yaml_point["target_position"][1].as<float>();
		p.target.z = yaml_point["target_position"][2].as<float>();

		points.push_back(p);
	}


	return true;
}


void ObservationStorage::detach_all_ghosts()
{
	lightcurves->detach_all_ghosts();
	ao_images->detach_all_ghosts();
	radar_images->detach_all_ghosts();
}
