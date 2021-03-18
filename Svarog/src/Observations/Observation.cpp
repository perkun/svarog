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



