#include <cppargs/cppargs.h>
#include "svpch.h"
#include "Application.h"
#include "RadarLayer.h"
#include "ObservationStorage.h"


using namespace std;


#define WIN_W 600
#define WIN_H 600

void init_app();
OrbitalComponent get_orbital_component_from_scene(string filepath);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	Parser parser;
	parser.add_option('m', "model", "model file (obj/shp)", false, "");
	Args args = parser.parse_args(argc, argv);


	init_app();
	RadarLayer *radar_layer = new RadarLayer(200);
	ObsStoragePack obs_pack;


	radar_layer->on_attach();

	if (args["model"])
		radar_layer->load_model(args.get_value<string>("model"));
	else
		radar_layer->load_model("../../../data/eros.obj");


	int obs_id = obs_pack.add_new("obs");
	vector<ObsPoint> obs_points =
		ObsStoragePack::import_obs_points("../../../data/radar/eros_flyby.storage");

	OrbitalComponent &target_oc =
		radar_layer->model.get_component<OrbitalComponent>();
	Transform &target_t = radar_layer->model.get_component<Transform>();
	Transform &observer_t = radar_layer->observer.get_component<Transform>();

	for (ObsPoint p : obs_points)
	{
		target_t.position = p.target_pos;
		observer_t.position = p.observer_pos;

		target_oc.calculate_rot_phase(p.jd);
		target_t.rotation = target_oc.xyz_from_lbg();

		radar_layer->make_radar_image(obs_pack.get_radar_images(obs_id), p.jd);
	}

	obs_pack.save_current( "../../../data/radar/obs", true);

	radar_layer->on_detach();
	Application::destroy();

	return 0;
}



void init_app()
{
	Application::init(WIN_W, WIN_H, "Application", false, false);
	Application::get_window()->set_cursor_normal();

	// load shaders
	#include "../shaders/basic.vs.include"
	#include "../shaders/basic.fs.include"
    basic_vs[basic_vs_len-1] = 0;
    basic_fs[basic_fs_len-1] = 0;

	#include "../shaders/radar.vs.include"
	#include "../shaders/radar.fs.include"
    radar_vs[radar_vs_len-1] = 0;
    radar_fs[radar_fs_len-1] = 0;

    shared_ptr<Shader> basic_shader = make_shared<Shader>();
    basic_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)basic_fs);

    shared_ptr<Shader> radar = make_shared<Shader>();
    radar->create_shader((char *)(void *)radar_vs,
                         (char *)(void *)radar_fs);

	Application::shaders["basic_shader"] = basic_shader;
	Application::shaders["radar"] = radar;
}


OrbitalComponent get_orbital_component_from_scene(string filepath)
{
    YAML::Node data = YAML::LoadFile(filepath);
    if (!data["Scene"])
	{
		cout << filepath << ", no such file" << endl;
        exit(1);
	}

    auto entities = data["Entities"];
    if (!entities)
    {
		cout << "Empty scene!" << endl;
        exit(1);
    }

	OrbitalComponent oc;
	for (auto entity : entities)
    {
        if (entity["OrbitalComponent"])
        {
            auto node = entity["OrbitalComponent"];

            oc.jd_0 = node["jd_0"].as<double>();
            oc.rotation_phase = node["rotation_phase"].as<double>();
            oc.rotation_speed = node["rotation_speed"].as<double>();
            oc.rot_period = node["rot_period"].as<double>();
            oc.lambda = node["lambda"].as<double>();
            oc.beta = node["beta"].as<double>();
            oc.gamma = node["gamma"].as<double>();
        }
    }
	return oc;
}
