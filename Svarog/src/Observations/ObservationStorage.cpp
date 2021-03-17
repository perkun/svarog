#include "svpch.h"
#include "ObservationStorage.h"


ObservationStorage::ObservationStorage()
{
	current_id = 0;
	obs_packs.emplace_back(ObsPack());

	obs_packs[current_id].lightcurves = new LightcurveSeries;
	obs_packs[current_id].ao_images = new ImageSeries;
	obs_packs[current_id].radar_images = new ImageSeries;
}


ObservationStorage::~ObservationStorage()
{
	for (int i = 0; i < obs_packs.size(); i++)
	{
		delete obs_packs[i].lightcurves;
		delete obs_packs[i].ao_images;
		delete obs_packs[i].radar_images;
	}

}

LightcurveSeries* ObservationStorage::get_current_lightcurves()
{
	if (obs_packs.size() > 0 && current_id >= 0 && current_id < obs_packs.size())
		return obs_packs[current_id].lightcurves;
	else
		return NULL;
}

ImageSeries* ObservationStorage::get_current_ao_images()
{
	if (obs_packs.size() > 0 && current_id >= 0 && current_id < obs_packs.size())
		return obs_packs[current_id].ao_images;
	else
		return NULL;
}


ImageSeries* ObservationStorage::get_current_radar_images()
{
	if (obs_packs.size() > 0 && current_id >= 0 && current_id < obs_packs.size())
		return obs_packs[current_id].radar_images;
	else
		return NULL;
}


LightcurveSeries* ObservationStorage::get_lightcurves(string name)
{
	for (int i = 0; i < obs_packs.size(); i++)
	{
		if (obs_packs[i].name == name)
			return obs_packs[i].lightcurves;
	}
	return NULL;
}


ImageSeries* ObservationStorage::get_ao_images(string name)
{
	for (int i = 0; i < obs_packs.size(); i++)
	{
		if (obs_packs[i].name == name)
			return obs_packs[i].ao_images;
	}
	return NULL;
}


ImageSeries* ObservationStorage::get_radar_images(string name)
{
	for (int i = 0; i < obs_packs.size(); i++)
	{
		if (obs_packs[i].name == name)
			return obs_packs[i].radar_images;
	}
	return NULL;
}




void ObservationStorage::save_current(const string filepath, bool export_obs)
{
    obs_packs[current_id].filename = File::remove_extension(filepath) + ".storage";
    obs_packs[current_id].name =
		fix_storage_name(File::remove_extension(File::file_base(filepath)), true);

	YAML::Emitter emitter;
	emitter << serialize(export_obs, filepath);

    std::ofstream fout(obs_packs[current_id].filename );
    fout << emitter.c_str();
    fout.close();
}


YAML::Node ObservationStorage::serialize(bool export_obs, string filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "points" << YAML::BeginSeq;

	if (export_obs)
	{
		obs_packs[current_id].lightcurves->serialize(out, filepath);
		obs_packs[current_id].ao_images->serialize(out, filepath);
		obs_packs[current_id].radar_images->serialize(out, filepath);
	}
	else
	{
		obs_packs[current_id].lightcurves->serialize(out);
		obs_packs[current_id].ao_images->serialize(out);
		obs_packs[current_id].radar_images->serialize(out);
	}

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

	return data;
}

void ObservationStorage::add_new(string name)
{
	name = fix_storage_name(name);

	obs_packs.emplace_back(ObsPack());
	obs_packs[size() - 1].lightcurves = new LightcurveSeries;
	obs_packs[size() - 1].ao_images = new ImageSeries;
	obs_packs[size() - 1].radar_images = new ImageSeries;

	obs_packs[size() -1].name = name;

    detach_current_ghosts();
    current_id = size() -1;
//     set_current_ghosts(obs_storage[current_storage]);
}


string ObservationStorage::get_current_name()
{
	return obs_packs[current_id].name;
}

string ObservationStorage::get_name(int id)
{
	if (obs_packs.size() > 0 && id >= 0 && id < obs_packs.size())
		return obs_packs[id].name;
	else
		return string("");
}


string ObservationStorage::fix_storage_name(string name, bool exclude_current)
{
    smatch matches;
	char buff[50];
	sprintf(buff, "(%s) ?\\(?([0-9]*)\\)?", name.c_str());
    for (int i = 0; i < size(); i++)
    {
		if (exclude_current)
			if (i == current_id)
				continue;

        if (regex_search(obs_packs[i].name, matches, regex(buff)) )
		{
			if (matches[2].str() == "")
				name = name + " (1)";
			else
			{
				stringstream ss;
				ss << matches[1] << " (" << stoi(matches[2].str()) +1 << ")";
				name = ss.str();
			}
		}
    }
    return name;
}


bool ObservationStorage::load(const string filename)
{
    YAML::Node data = YAML::LoadFile(filename);
    if (!data["points"])
        return false;

	detach_current_ghosts();
	add_new(File::remove_extension(File::file_base(filename)));
    obs_packs[current_id].file_loaded = false;
    obs_packs[current_id].filename = filename;

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
                if (t.as<string>() == "lc")
                    p.obs_types |= ObsType::LC;
                else if (t.as<string>() == "ao")
                    p.obs_types |= ObsType::AO;
                else if (t.as<string>() == "radar")
                    p.obs_types |= ObsType::RADAR;
        }
        else
            p.obs_types |= ObsType::LC;

		if (yaml_point["lc_num_points"])
			p.lc_num_points = yaml_point["lc_num_points"].as<int>();

		if (yaml_point["ao_size"])
			p.ao_size = yaml_point["ao_size"].as<int>();


        obs_packs[current_id].points.push_back(p);
    }

    obs_packs[current_id].file_loaded = true;
    return true;
}


vector<YamlPoint> ObservationStorage::get_current_points()
{
	 return obs_packs[current_id].points;
}


int ObservationStorage::get_current_points_size()
{
	 return obs_packs[current_id].points.size();
}


void ObservationStorage::detach_current_ghosts()
{
    obs_packs[current_id].lightcurves->detach_all_ghosts();
    obs_packs[current_id].ao_images->detach_all_ghosts();
    obs_packs[current_id].radar_images->detach_all_ghosts();
}


void ObservationStorage::delete_current_observations()
{
    while (get_current_lightcurves()->get_current_obs() != NULL)
        get_current_lightcurves()->delete_current_obs();

    while (get_current_ao_images()->get_current_obs() != NULL)
        get_current_ao_images()->delete_current_obs();

    while (get_current_radar_images()->get_current_obs() != NULL)
        get_current_radar_images()->delete_current_obs();
}


void ObservationStorage::delete_current()
{
	if (size() == 1)
		delete_current_observations();
	else
	{
		delete obs_packs[current_id].lightcurves;
		delete obs_packs[current_id].ao_images;
		delete obs_packs[current_id].radar_images;

		obs_packs.erase(obs_packs.begin() + current_id);
		current_id--;

		if (current_id < 0)
			current_id = 0;
	}
}
