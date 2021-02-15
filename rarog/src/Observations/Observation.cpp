#include "svpch.h"
#include "Observation.h"


Observation::Observation(Entity &target, Entity &observer)
{
	this->target = target;
	this->observer = observer;
	target_pos = target.get_component<Transform>().position;
	observer_pos = observer.get_component<Transform>().position;
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
        .add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = ghost_color;
    ghost_observer.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(0.03))));
    Transform &got = ghost_observer.get_component<Transform>();
    got.position = observer.get_component<Transform>().position;

    ghost_target
        .add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = ghost_color;
    ghost_target.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedCube(vec3(-.025), vec3(0.05))));
    Transform &gtt = ghost_target.get_component<Transform>();
    gtt.position = target.get_component<Transform>().position;
}


Image::Image(Entity &target, Entity &observer, int width, int height)
	: Observation(target, observer)
{

	texture_specs.target = GL_TEXTURE_2D;
	texture_specs.level = 0;
	texture_specs.internal_format = GL_RGB32F;
	texture_specs.width = width;
	texture_specs.height = height;
	texture_specs.depth = 1;
	texture_specs.border = 0;
	texture_specs.format = GL_RGB;
	texture_specs.type = GL_FLOAT;

	texture = make_shared<Texture>(texture_specs);

	ghost_color = vec4(237 / 256., 84 / 256., 84 / 256., 0.2);
}


Image::~Image()
{
}
