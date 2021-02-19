#include "svpch.h"
#include "Material.h"

Material::Material()
{
}

Material::Material(string shader_name) : shader_name(shader_name)
{
	shader = Application::shaders[shader_name];
}


Material::~Material()
{
}

void Material::set_uniforms()
{
    shader->bind();

	for (auto u: uniforms_vec3)
    {
		shader->set_uniform_3fv(u.first, u.second);
	}

	for (auto u: uniforms_vec4)
    {
		shader->set_uniform_4fv(u.first, u.second);
	}

	for (auto u: uniforms_mat4)
	{
		shader->set_uniform_mat4f(u.first, u.second);
	}

	for (auto u: uniforms_int)
	{
		shader->set_uniform_1i(u.first, u.second);
	}

	for (auto u: uniforms_float)
	{
		shader->set_uniform_1f(u.first, u.second);
	}
}

void Material::set_uniforms(shared_ptr<Shader> shader)
{
    shader->bind();

	for (auto u: uniforms_vec3)
    {
		shader->set_uniform_3fv(u.first, u.second);
	}

	for (auto u: uniforms_vec4)
    {
		shader->set_uniform_4fv(u.first, u.second);
	}

	for (auto u: uniforms_mat4)
	{
		shader->set_uniform_mat4f(u.first, u.second);
	}

	for (auto u: uniforms_int)
	{
		shader->set_uniform_1i(u.first, u.second);
	}

	for (auto u: uniforms_float)
	{
		shader->set_uniform_1f(u.first, u.second);
	}
}
