#include "svpch.h"
#include "ObservationStorage.h"


ObsStoragePack::ObsStoragePack()
{
	current_id = 0;
	obs_storages.emplace_back(ObsStorage());
}


ObsStoragePack::~ObsStoragePack()
{
	for (int i = 0; i < obs_storages.size(); i++)
	{
		for (auto elem : obs_storages[i].storage)
			delete elem.second;
	}
}



void ObsStoragePack::save_current(const string filepath, bool export_obs)
{
	save(current_id, filepath, export_obs);
}


void ObsStoragePack::save(int id, const string filepath, bool export_obs)
{
    string filename = File::remove_extension(filepath) + ".storage";
    obs_storages[id].name =
		fix_storage_name(File::remove_extension(File::file_base(filepath)), true);

	YAML::Emitter emitter;
	emitter << serialize(id, export_obs, filepath);

    std::ofstream fout(filename);
    fout << emitter.c_str();
    fout.close();
}


YAML::Node ObsStoragePack::serialize(int id, bool export_obs, string filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "points" << YAML::BeginSeq;

	if (export_obs)
	{
		for (auto elem : obs_storages[id].storage)
			elem.second->serialize(out, filepath);
	}
	else
	{
		for (auto elem : obs_storages[id].storage)
			elem.second->serialize(out);
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

int ObsStoragePack::add_new(string name)
{
	name = fix_storage_name(name);

	obs_storages.emplace_back(ObsStorage());
    current_id = size() -1;

	obs_storages[current_id].name = name;

    detach_all_ghosts();
	return current_id;
}


string ObsStoragePack::get_current_name()
{
	return obs_storages[current_id].name;
}

string ObsStoragePack::get_name(int id)
{
	if (obs_storages.size() > 0 && id >= 0 && id < obs_storages.size())
		return obs_storages[id].name;
	else
		return string("");
}


string ObsStoragePack::fix_storage_name(string name, bool exclude_current)
{
    smatch matches;
	char buff[50];
	sprintf(buff, "(%s) ?\\(?([0-9]*)\\)?", name.c_str());
    for (int i = 0; i < size(); i++)
    {
		if (exclude_current)
			if (i == current_id)
				continue;

        if (regex_search(obs_storages[i].name, matches, regex(buff)) )
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


vector<ObsPoint> ObsStoragePack::import_obs_points(const string filename)
{
    YAML::Node data = YAML::LoadFile(filename);
    if (!data["points"])
        return vector<ObsPoint>();

	vector<ObsPoint> points;
    for (auto yaml_point : data["points"])
    {
        ObsPoint p;
        p.jd = yaml_point["jd"].as<double>();

        p.observer_pos.x = yaml_point["observer_position"][0].as<float>();
        p.observer_pos.y = yaml_point["observer_position"][1].as<float>();
        p.observer_pos.z = yaml_point["observer_position"][2].as<float>();

        p.target_pos.x = yaml_point["target_position"][0].as<float>();
        p.target_pos.y = yaml_point["target_position"][1].as<float>();
        p.target_pos.z = yaml_point["target_position"][2].as<float>();

        if (yaml_point["type"])
        {
            for (auto t : yaml_point["type"])
                if (t.as<string>() == "lc")
                    p.obs_type |= ObsType::LC;
                else if (t.as<string>() == "ao")
                    p.obs_type |= ObsType::AO;
                else if (t.as<string>() == "radar")
                    p.obs_type |= ObsType::RADAR;
        }
        else
            p.obs_type |= ObsType::LC;

        if (yaml_point["lc_num_points"])
            p.lc_num_points = yaml_point["lc_num_points"].as<int>();

        if (yaml_point["ao_size"])
            p.ao_size = yaml_point["ao_size"].as<int>();

		if (yaml_point["radar_image"])
			p.radar_image_filename = yaml_point["radar_image"].as<string>();

		if (yaml_point["radar_fits"])
			p.radar_fits_filename = yaml_point["radar_fits"].as<string>();

		if (yaml_point["ao_image"])
			p.ao_image_filename = yaml_point["ao_image"].as<string>();

		if (yaml_point["ao_fits"])
			p.ao_fits_filename = yaml_point["ao_fits"].as<string>();

		if (yaml_point["mag_data"])
			p.mag_filename = yaml_point["mag_data"].as<string>();

		if (yaml_point["flux_data"])
			p.flux_filename = yaml_point["flux_data"].as<string>();

        points.push_back(p);
    }

    return points;
}

void ObsStoragePack::detach_all_ghosts()
{
    for (ObsStorage &obs_storage : obs_storages)
        for (auto elem : obs_storage.storage)
            elem.second->detach_all_ghosts();
}

void ObsStoragePack::detach_current_ghosts()
{

	for (auto elem : obs_storages[current_id].storage)
		elem.second->detach_all_ghosts();
}


void ObsStoragePack::delete_current_observations()
{
	for (auto elem : obs_storages[current_id].storage)
		while (elem.second->get_current_obs() != NULL)
			elem.second->delete_current_obs();
}


void ObsStoragePack::delete_current()
{
	if (size() == 1)
		delete_current_observations();
	else
	{
		for (auto elem : obs_storages[current_id].storage)
			delete elem.second;

		obs_storages.erase(obs_storages.begin() + current_id);
		current_id--;

		if (current_id < 0)
			current_id = 0;
	}
}


void ObsStoragePack::current_obs_lambda(const function<void(Observation *)> &func)
{
	for (auto elem: obs_storages[current_id].storage)
	{
		func(elem.second->get_current_obs());
	}
}
