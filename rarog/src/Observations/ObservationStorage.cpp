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

    lightcurves->serialize(out);
    ao_images->serialize(out);
    radar_images->serialize(out);

    out << YAML::EndSeq;
    out << YAML::EndMap;

    // 1. emiter to node
    YAML::Node data = YAML::Load(out.c_str());
    YAML::Node ps = data["points"];

    // 2. check for overlaping points
    int psize = data["points"].size();
    for (int i = 0; i < psize;)
    {
        for (int j = i + 1; j < psize;)
        {
            if (ps[i]["jd"].as<double>() == ps[j]["jd"].as<double>() &&
                ps[i]["target_position"][0].as<double>() ==
                    ps[j]["target_position"][0].as<double>() &&
                ps[i]["target_position"][1].as<double>() ==
                    ps[j]["target_position"][1].as<double>() &&
                ps[i]["target_position"][2].as<double>() ==
                    ps[j]["target_position"][2].as<double>() &&

                ps[i]["observer_position"][0].as<double>() ==
                    ps[j]["observer_position"][0].as<double>() &&
                ps[i]["observer_position"][1].as<double>() ==
                    ps[j]["observer_position"][1].as<double>() &&
                ps[i]["observer_position"][2].as<double>() ==
                    ps[j]["observer_position"][2].as<double>() )
            {
				bool skip = false;
				for (int k = 0; k < ps[i]["type"].size(); k++)
					if (ps[j]["type"][0].as<string>() == ps[i]["type"][k].as<string>())
						skip = true;
				if (skip)
				{
					j++;
					continue;
				}

                // merge fields
                for (auto it = ps[j].begin(); it != ps[j].end(); ++it)
                {
                    if (!ps[i][it->first.as<string>().c_str()].IsDefined())
                        ps[i][it->first.as<string>().c_str()] =
                            ps[j][it->first.as<string>().c_str()];
                }
                // add type (should only be one in ps[j]???)
                ps[i]["type"].push_back(ps[j]["type"][0]);

                ps.remove(j);
                psize--;
                continue;
            }
            j++;
        }


        i++;
    }

    // 3. merge
    // 4. node to emiter, and save
    YAML::Emitter emitter;
    emitter << data;

    std::ofstream fout(filepath);
    fout << emitter.c_str();
    fout.close();
}


bool ObservationStorage::load(const string filename)
{
    file_loaded = false;
    this->filename = filename;
    this->name = File::remove_extension(File::file_base(filename));


    YAML::Node data = YAML::LoadFile(filename);
    if (!data["points"])
        return false;


    for (auto yaml_point : data["points"])
    {
        YamlPoint p;
        p.jd = yaml_point["jd"].as<double>();

        p.observer.x = yaml_point["observer_position"][0].as<float>();
        p.observer.y = yaml_point["observer_position"][1].as<float>();
        p.observer.z = yaml_point["observer_position"][2].as<float>();

        p.target.x = yaml_point["target_position"][0].as<float>();
        p.target.y = yaml_point["target_position"][1].as<float>();
        p.target.z = yaml_point["target_position"][2].as<float>();

        if (yaml_point["type"])
        {
            for (auto t : yaml_point["type"])
                if (t.as<string>() == "LC")
                    p.obs_types |= ObsType::LC;
                else if (t.as<string>() == "AO")
                    p.obs_types |= ObsType::AO;
                else if (t.as<string>() == "RADAR")
                    p.obs_types |= ObsType::RADAR;
        }
        else
            p.obs_types |= ObsType::LC;

		if (yaml_point["lc_num_points"])
			p.lc_num_points = yaml_point["lc_num_points"].as<int>();

		if (yaml_point["ao_size"])
			p.ao_size = yaml_point["ao_size"].as<int>();


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
