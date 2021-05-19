#include "svpch.h"
#include "ObservationStorage.h"


ObsStoragePack::ObsStoragePack()
{
	current_id = 0;
	obs_pack.emplace_back(ObsStorage());
}


ObsStoragePack::~ObsStoragePack()
{
	for (int i = 0; i < obs_pack.size(); i++)
	{
		for (auto elem : obs_pack[i].series_map)
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
    obs_pack[id].name =
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
		for (auto elem : obs_pack[id].series_map)
		{
			elem.second->serialize(out, elem.first, filepath);
		}
	}
	else
	{
		for (auto elem : obs_pack[id].series_map)
			elem.second->serialize(out, elem.first);
	}

    out << YAML::EndSeq;
    out << YAML::EndMap;

    // 1. emiter to node
    YAML::Node data = YAML::Load(out.c_str());
    YAML::Node ps = data["points"];

    // 2. check for overlaping points
//     int psize = data["points"].size();
//     for (int i = 0; i < psize;)
//     {
//         for (int j = i + 1; j < psize;)
//         {
//             if (ps[i]["jd"].as<double>() == ps[j]["jd"].as<double>() &&
//                 ps[i]["target_position"][0].as<double>() ==
//                     ps[j]["target_position"][0].as<double>() &&
//                 ps[i]["target_position"][1].as<double>() ==
//                     ps[j]["target_position"][1].as<double>() &&
//                 ps[i]["target_position"][2].as<double>() ==
//                     ps[j]["target_position"][2].as<double>() &&
//
//                 ps[i]["observer_position"][0].as<double>() ==
//                     ps[j]["observer_position"][0].as<double>() &&
//                 ps[i]["observer_position"][1].as<double>() ==
//                     ps[j]["observer_position"][1].as<double>() &&
//                 ps[i]["observer_position"][2].as<double>() ==
//                     ps[j]["observer_position"][2].as<double>() )
//             {
// 				bool skip = false;
// 				for (int k = 0; k < ps[i]["type"].size(); k++)
// 					if (ps[j]["type"][0].as<string>() == ps[i]["type"][k].as<string>())
// 						skip = true;
// 				if (skip)
// 				{
// 					j++;
// 					continue;
// 				}
//
//                 // merge fields
//                 for (auto it = ps[j].begin(); it != ps[j].end(); ++it)
//                 {
//                     if (!ps[i][it->first.as<string>().c_str()].IsDefined())
//                         ps[i][it->first.as<string>().c_str()] =
//                             ps[j][it->first.as<string>().c_str()];
//                 }
//                 // add type (should only be one in ps[j]???)
//                 ps[i]["type"].push_back(ps[j]["type"][0]);
//
//                 ps.remove(j);
//                 psize--;
//                 continue;
//             }
//             j++;
//         }
//         i++;
//     }

	return data;
}

int ObsStoragePack::add_new_storage(string name)
{
	name = fix_storage_name(name);

	obs_pack.emplace_back(ObsStorage());
    current_id = size() -1;

	obs_pack[current_id].name = name;

    detach_all_ghosts();
	return current_id;
}


string ObsStoragePack::get_current_name()
{
	return obs_pack[current_id].name;
}

string ObsStoragePack::get_name(int id)
{
	if (obs_pack.size() > 0 && id >= 0 && id < obs_pack.size())
		return obs_pack[id].name;
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

        if (regex_search(obs_pack[i].name, matches, regex(buff)) )
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


vector<ObsPoint> ObsStoragePack::deserialize_storage(const string filename)
{
    YAML::Node data = YAML::LoadFile(filename);
    if (!data["points"])
        return vector<ObsPoint>();


    add_new_storage(File::remove_extension(File::file_base(filename)));

    vector<ObsPoint> points;
    for (auto yaml_obs : data["points"])
    {
        ObsPoint p;
        p.jd = yaml_obs["jd"].as<double>();

        p.observer_pos.x = yaml_obs["observer_position"][0].as<float>();
        p.observer_pos.y = yaml_obs["observer_position"][1].as<float>();
        p.observer_pos.z = yaml_obs["observer_position"][2].as<float>();

        p.target_pos.x = yaml_obs["target_position"][0].as<float>();
        p.target_pos.y = yaml_obs["target_position"][1].as<float>();
        p.target_pos.z = yaml_obs["target_position"][2].as<float>();

        if (yaml_obs["type"])
        {
            if (yaml_obs["type"].as<string>() == "lc")
                p.obs_type |= ObsType::LC;
            else if (yaml_obs["type"].as<string>() == "ao")
                p.obs_type |= ObsType::AO;
            else if (yaml_obs["type"].as<string>() == "radar")
                p.obs_type |= ObsType::RADAR;
        }
        else
            p.obs_type |= ObsType::LC;

        if (yaml_obs["lc_num_points"])
            p.lc_num_points = yaml_obs["lc_num_points"].as<int>();

        if (yaml_obs["ao_size"])
            p.ao_size = yaml_obs["ao_size"].as<int>();

        if (yaml_obs["radar_image"])
            p.radar_image_filename = yaml_obs["radar_image"].as<string>();

        if (yaml_obs["radar_fits"])
            p.radar_fits_filename = yaml_obs["radar_fits"].as<string>();

        if (yaml_obs["ao_image"])
            p.ao_image_filename = yaml_obs["ao_image"].as<string>();

        if (yaml_obs["ao_fits"])
            p.ao_fits_filename = yaml_obs["ao_fits"].as<string>();

        if (yaml_obs["mag_data"])
            p.mag_filename = yaml_obs["mag_data"].as<string>();

        if (yaml_obs["flux_data"])
            p.flux_filename = yaml_obs["flux_data"].as<string>();

        if (yaml_obs["storage_name"])
            p.storage_name = yaml_obs["storage_name"].as<string>();

        points.push_back(p);
    }


    return points;
}

vector<ObsPoint> ObsStoragePack::deserialize_damit_lc(string filepath,
													  Entity &target,
													  Entity &observer)
{
    /// The lightcurves are in phase space. The 0 phase in the plot is the phase
    /// of the model as it would be observed at this time. So, the observed
    /// points always start at 0

	if (filepath == "")
        return vector<ObsPoint>();

    char line[1000];
    int num_lcs = 0;
    vector<ObsPoint> obs_points;

    FILE *in = fopen(filepath.c_str(), "r");

    fgets(line, 999, in);
    sscanf(line, "%d", &num_lcs);

    if (num_lcs > 0)
    {
        add_new_storage(File::remove_extension(File::file_base(filepath)));
        add_series<LightcurveSeries>("obs_lightcurves");
    }
    else
        return vector<ObsPoint>();

    OrbitalComponent &oc =
        target.get_component<OrbitalComponent>();

    for (int i = 0; i < num_lcs; i++)
    {
        fgets(line, 999, in);
        int type = 0;
        int num_pts = 0;
        sscanf(line, "%d %d", &num_pts, &type);

        Lightcurve *lc =
            new Lightcurve(target, observer, num_pts);

		double phase_offset = 0;
		double jd_start = 0;
        for (int j = 0; j < num_pts; j++)
        {
            double jd, flux;
            dvec3 sun, earth;
            fgets(line, 999, in);
            sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &jd, &flux, &sun.x,
                   &sun.y, &sun.z, &earth.x, &earth.y, &earth.z);

            if (j == 0)
            {
				jd_start = jd;
                lc->julian_day = jd;

                ObsPoint p;
                p.jd = jd;
                p.observer_pos = earth - sun;
                p.target_pos = -sun;
                p.obs_type = ObsType::LC;
				p.storage_name = "lightcurves";

                obs_points.push_back(p);
            }

			double phase = (jd - jd_start) / (oc.rot_period / 24.) *2 * M_PI;
			while (phase >= 2*M_PI)
				phase -= 2*M_PI;
			while (phase < 0)
				phase += 2* M_PI;

            lc->push_flux(phase, flux);

        }

        lc->sort();
        get_series<LightcurveSeries>("obs_lightcurves")->push(lc);
    }
    fclose(in);

	return obs_points;

}


void ObsStoragePack::detach_all_ghosts()
{
    for (ObsStorage &obs_storage : obs_pack)
        for (auto elem : obs_storage.series_map)
            elem.second->detach_all_ghosts();
}

void ObsStoragePack::detach_current_ghosts()
{

	for (auto elem : obs_pack[current_id].series_map)
		elem.second->detach_all_ghosts();
}


void ObsStoragePack::delete_current_observations()
{
	for (auto elem : obs_pack[current_id].series_map)
		while (elem.second->get_current_obs() != NULL)
			elem.second->delete_current_obs();
}


void ObsStoragePack::delete_current()
{
	if (size() == 1)
		delete_current_observations();
	else
	{
		for (auto elem : obs_pack[current_id].series_map)
			delete elem.second;

		obs_pack.erase(obs_pack.begin() + current_id);
		current_id--;

		if (current_id < 0)
			current_id = 0;
	}
}


void ObsStoragePack::current_obs_lambda(const function<void(Observation *)> &func)
{
	for (auto elem: obs_pack[current_id].series_map)
	{
		func(elem.second->get_current_obs());
	}
}
