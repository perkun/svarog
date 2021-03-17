#include "svpch.h"
#include "Observation.h"

Observation::Observation(Entity &target, Entity &observer)
{
	this->target = target;
	this->observer = observer;
	target_transform = target.get_component<Transform>();
	observer_transform = observer.get_component<Transform>();
	if (target.has_component<OrbitalComponent>())
		target_orbital_component = target.get_component<OrbitalComponent>();
}


Observation::~Observation()
{
	ghost_observer.destroy();
	ghost_target.destroy();
}


FitsHeader Observation::get_fits_header()
{
	FitsHeader header;
	push_basic_header_info(header);
	return header;
}

void Observation::push_basic_header_info(FitsHeader &header)
{
    header.push("obs_x", observer_transform.position.x,
                 "observer x position [au]");
    header.push("obs_y", observer_transform.position.y,
                 "observer y position [au]");
    header.push("obs_z", observer_transform.position.z,
                 "observer z position [au]");

    header.push("pos_x", target_transform.position.x,
                 "target x position [au]");
    header.push("pos_y", target_transform.position.y,
                 "target y position [au]");
    header.push("pos_z", target_transform.position.z,
                 "target z position [au]");

    header.push("lambda", target_orbital_component.lambda,
                 "lambda angle [rad]");
    header.push("beta", target_orbital_component.beta, "beta angle [rad]");
    header.push("gamma", target_orbital_component.gamma, "gamma angle [rad]");

    header.push("phase", target_orbital_component.rotation_phase,
                 "rotation phase [rad]");

    header.push("period", target_orbital_component.rot_period,
                 "rotation period [h]");

    header.push("jd_0", target_orbital_component.jd_0,
                 "reference eopch for gamma=0 [julian day]");

    header.push("jd", julian_day, "image epoch");
}


void Observation::serialize(YAML::Emitter &out, int id, string filename)
{
	out << YAML::BeginMap;

	out << YAML::Key << "jd" << YAML::Value << julian_day;

	out << YAML::Key << "target_position" << YAML::BeginSeq;
	out << YAML::Value << target_transform.position.x;
	out << YAML::Value << target_transform.position.y;
	out << YAML::Value << target_transform.position.z << YAML::EndSeq;

	out << YAML::Key << "observer_position" << YAML::BeginSeq;
	out << YAML::Value << observer_transform.position.x;
	out << YAML::Value << observer_transform.position.y;
	out << YAML::Value << observer_transform.position.z << YAML::EndSeq;

	out << YAML::Key << "type";
	out << YAML::BeginSeq <<  YAML::Value << "GENERIC" << YAML::EndSeq;


	out << YAML::EndMap;
}



void Observation::add_ghosts(Entity &gt, Entity &go)
{
	ghost_target = gt;
	ghost_observer = go;

    ghost_observer
        .add_component<Material>("flat_shader")
        .uniforms_vec4["u_color"] = ghost_color;
    ghost_observer.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(0.03))));
    Transform &got = ghost_observer.get_component<Transform>();
    got.position = observer.get_component<Transform>().position;

    ghost_target
        .add_component<Material>("flat_shader")
        .uniforms_vec4["u_color"] = ghost_color;
    ghost_target.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedCube(vec3(-.025), vec3(0.05))));
    Transform &gtt = ghost_target.get_component<Transform>();
    gtt.position = target.get_component<Transform>().position;
}

string Observation::get_obs_type_string()
{
	return string("generic");
}


